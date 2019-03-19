#include "Framework.h"
#include "ScriptInstance.h"
#include "ScriptModule.h"
#include <AngelScript_2.33.0/angelscript.h>
#include "../Scene/GameObject.h"

ScriptInstance::ScriptInstance()
	: scripting(nullptr),
	scriptObject(nullptr),
	startFunc(nullptr),
	updateFunc(nullptr),
	constructorFunc(nullptr),
	scriptModule(nullptr),
	object(nullptr),
	scriptPath(""),
	className(""),
	constructorDeclaration(""),
	moduleName(""),
	bInstantiated(false)
{
}

ScriptInstance::~ScriptInstance()
{
	if (scriptObject) {
		scriptObject->Release();
		scriptObject = nullptr;
	}

	constructorFunc = nullptr;
	startFunc = nullptr;
	updateFunc = nullptr;
	scripting = nullptr;
	bInstantiated = false;
}

auto ScriptInstance::Instantiate(const std::string & path, GameObject * object, Scripting * scripting) -> const bool
{
	if (!object) {
		Log::Error("ScriptInstance : Instantiate - Object is Empty");
		return false;
	}

	this->scripting = scripting;
	this->scriptPath = path;
	this->object = object;
	this->className = FileSystem::GetIntactFileNameFromFilePath(path);
	this->moduleName = className + std::to_string(object->GetID());
	this->constructorDeclaration = className + " @" + className + "(GameObject @)"; //생성자. @는 angelscript에서 &. class @class(GameObject @)

	bInstantiated = CreateScriptObject();
	return bInstantiated;
}

void ScriptInstance::ExecuteStart()
{
	if (!scripting) {
		Log::Error("ScriptInstance : ExcuteStart - scripting is Empty");
		return;
	}

	scripting->ExecuteCall(startFunc, scriptObject);
}

void ScriptInstance::ExecuteUpdate()
{
	if (!scripting) {
		Log::Error("ScriptInstance : ExecuteUpdate - scripting is Empty");
		return;
	}

	scripting->ExecuteCall(updateFunc, scriptObject);
}

auto ScriptInstance::CreateScriptObject() -> const bool
{
	if (!scripting) {
		Log::Error("ScriptInstance : CreateScriptObject - scripting is Empty");
		return false;
	}

	//create Module
	scriptModule = new ScriptModule(scripting, moduleName);
	auto bResult = scriptModule->LoadScript(scriptPath);

	if (!bResult) {
		Log::Error("ScriptInstance : CreateScriptObject - Failed to Load Script");
		return false;
	}

	//Get Type
	auto type_id = scriptModule->GetAsIScriptModule()->GetTypeIdByDecl(className.c_str());
	asITypeInfo *type = scripting->GetAsIScriptEngine()->GetTypeInfoById(type_id);

	if (!type) {
		Log::Error("ScriptInstance : CreateScriptObject - Incorrect Type");
		return false;
	}

	//Get Function in the script
	startFunc = type->GetMethodByDecl("void start()");
	updateFunc = type->GetMethodByDecl("void Update()");
	constructorFunc = type->GetFactoryByDecl(constructorDeclaration.c_str());

	if (!constructorFunc) {
		Log::Error("ScriptInstance : CreateScriptObject - not found approriate factory for the type" + className + "(line 106)");
		return false;
	}

	asIScriptContext *ctx = scripting->RequestContext();
	{
		int result = ctx->Prepare(constructorFunc);
		if (result < 0) return false;

		result = ctx->SetArgObject(0, object);
		if (result < 0) return false;

		result = ctx->Execute();
		if (result < 0) return false;

		scriptObject = *reinterpret_cast<asIScriptObject**>(ctx->GetAddressOfReturnValue());
		scriptObject->AddRef(); //reference count 증가. 쓰고 있음을 알리기 위함
	}
	scripting->ReturnContext(ctx);

	return true;
}
