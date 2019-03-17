#include "Framework.h"
#include "ScriptInterface.h"
#include <AngelScript_2.33.0/angelscript.h>

void ScriptInterface::Register(Context * context, asIScriptEngine * scriptEngine)
{
	RegisterEnumerations();
	RegisterTypes();
	RegisterSettings();
	RegisterTimer();
	RegisterInput();
	RegisterGameObject();
	RegisterTransform();
	RegisterRenderable();
	RegisterCamera();
	RegisterMaterial();
	RegisterMesh();
	RegisterMath();
	RegisterVector2();
	RegisterVector3();
}

void ScriptInterface::RegisterEnumerations()
{
}

void ScriptInterface::RegisterTypes()
{
}

void ScriptInterface::RegisterSettings()
{
}

void ScriptInterface::RegisterTimer()
{
}

void ScriptInterface::RegisterInput()
{
}

void ScriptInterface::RegisterGameObject()
{
}

void ScriptInterface::RegisterTransform()
{
}

void ScriptInterface::RegisterRenderable()
{
}

void ScriptInterface::RegisterCamera()
{
}

void ScriptInterface::RegisterMaterial()
{
}

void ScriptInterface::RegisterMesh()
{
}

void ScriptInterface::RegisterMath()
{
}

void ScriptInterface::RegisterVector2()
{
}

void ScriptInterface::RegisterVector3()
{
}
