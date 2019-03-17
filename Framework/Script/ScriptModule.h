#pragma once

class ScriptModule final
{
public:
	ScriptModule(class Scripting *scriptingm, const std::string &moduleName);
	~ScriptModule();

	ScriptModule(const ScriptModule&) = delete;
	ScriptModule &operator=(const ScriptModule&) = delete;

	auto LoadScript(const std::string &path) -> const bool;
	auto GetAsIScriptModule() -> class asIScriptModule*;

private:
	class CScriptBuilder *builder;
	class Scripting *scripting;
	std::string moduleName;
};