#pragma once

class ScriptInstance final
{
public:
	ScriptInstance();
	~ScriptInstance();

	ScriptInstance(const ScriptInstance&) = delete;
	ScriptInstance &operator=(const ScriptInstance&) = delete;

	auto Instantiate(const std::string &path, class GameObject *object, class Scripting *scripting)->const bool;
	auto GetScriptPath() const -> const std::string& { return scriptPath; }
	auto IsIntantiated() const -> const bool& { return bInstantiated; }

	void ExecuteStart();
	void ExecuteUpdate();

private:
	auto CreateScriptObject() -> const bool;

private:
	class Scripting *scripting;
	class asIScriptObject *scriptObject;
	class asIScriptFunction *startFunc;
	class asIScriptFunction *updateFunc;
	class asIScriptFunction *constructorFunc;
	class ScriptModule *scriptModule;
	class GameObject *object;
	std::string scriptPath;
	std::string className;
	std::string constructorDeclaration;
	std::string moduleName;
	bool bInstantiated;
};