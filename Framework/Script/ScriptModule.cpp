#include "Framework.h"
#include "ScriptModule.h"
#include <AngelScript_2.33.0/scriptbuilder/scriptbuilder.cpp>

ScriptModule::ScriptModule(Scripting *scripting, const std::string & moduleName) : builder(nullptr), scripting(scripting), moduleName(moduleName)
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

	if (!builder) builder = new CScriptBuilder();

	//start new module
	auto result = builder->StartNewModule(scripting->GetAsIScriptEngine(), moduleName.c_str());
	if (result < 0) {
		Log::Error("ScriptModule : GetAsIScriptModule - Failed to Start new Module");
		return false;
	}

	//load the script
	result = builder->AddSectionFromFile(path.c_str());
	if (result < 0) {
		Log::WriteFormatError("ScriptModule : GetAsIScriptModule - Failed to Load script \"%s\"", path);
		return false;
	}

	//build the script
	result = builder->BuildModule();
	if (result < 0) {
		Log::WriteFormatError("ScriptModule : GetAsIScriptModule - Failed to Compile script \"%s\"", FileSystem::GetFileNameFromFilePath(path));
		return false;
	}

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
