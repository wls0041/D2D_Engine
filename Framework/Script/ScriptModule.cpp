#include "Framework.h"
#include "ScriptModule.h"
#include <AngelScript_2.33.0/scriptbuilder/scriptbuilder.cpp>

ScriptModule::ScriptModule(Scripting *scriptingm, const std::string & moduleName) : builder(nullptr), scripting(scripting), moduleName(moduleName)
{
}

ScriptModule::~ScriptModule()
{
	scripting->DiscardModule(moduleName);
	SAFE_DELETE(builder);
}

auto ScriptModule::LoadScript(const std::string & path) -> const bool
{
	if (!scripting) {
		Log::Error("ScriptModule : LoadScript - scripting is Empty");
		return false;
	}
	//start new module
	auto result = builder->StartNewModule(scripting->GetAsIScriptEngine(), moduleName.c_str());
	assert(result >= 0);

	//load the script
	result = builder->AddSectionFromFile(path.c_str());
	assert(result >= 0);

	//build the script
	result = builder->BuildModule();
	assert(result >= 0);

	return true;
}

auto ScriptModule::GetAsIScriptModule() -> class asIScriptModule *
{
	if (!builder) {
		Log::Error("ScriptModule : GetAsIScriptModule - Builder is Empty");
		return nullptr;
	}

	return builder->GetModule();
}
