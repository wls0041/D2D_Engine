#pragma once

class ScriptInterface final
{
public:
	ScriptInterface() = default;
	~ScriptInterface() = default;

	ScriptInterface(const ScriptInterface&) = delete;
	ScriptInterface &operator=(const ScriptInterface&) = delete;

	void Register(class Context *context, class asIScriptEngine *scriptEngine);

private:
	void RegisterEnumerations();
	void RegisterTypes();
	void RegisterLog();
	void RegisterSettings();
	void RegisterTimer();
	void RegisterInput();
	void RegisterGameObject();
	void RegisterTransform();
	void RegisterRenderable();
	void RegisterCamera();
	void RegisterMaterial();
	void RegisterMesh();
	void RegisterMath();
	void RegisterVector2();
	void RegisterVector3();

private:
	class Context *context;
	class asIScriptEngine *scriptEngine;
};