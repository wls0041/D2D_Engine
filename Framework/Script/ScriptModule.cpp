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
	return false;
}

auto ScriptModule::GetAsIScriptModule() -> class asIScriptModule *
{
	return nullptr;
}
