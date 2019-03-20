#include "Framework.h"
#include "ScriptInterface.h"
#include <AngelScript_2.33.0/angelscript.h>
#include "../Scene/GameObject.h"
#include "../Scene/Component/Camera.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Renderable.h"

void ScriptInterface::Register(Context * context, asIScriptEngine * scriptEngine)
{
	this->context = context;
	this->scriptEngine = scriptEngine;

	RegisterEnumerations();
	RegisterTypes();
	RegisterVector2();
	RegisterVector3();
	//RegisterLog();
	//RegisterSettings();
	RegisterTimer();
	RegisterInput();
	RegisterTransform();
	RegisterGameObject();
	//RegisterRenderable();
	//RegisterCamera();
	//RegisterMaterial();
	//RegisterMesh();
	//RegisterMath();
}

void ScriptInterface::RegisterEnumerations()
{
	// Log
	scriptEngine->RegisterEnum("LogType");
	scriptEngine->RegisterEnumValue("LogType", "Info", static_cast<uint>(LogType::Info));
	scriptEngine->RegisterEnumValue("LogType", "Warning", static_cast<uint>(LogType::Warning));
	scriptEngine->RegisterEnumValue("LogType", "Error", static_cast<uint>(LogType::Error));


	// Component types
	scriptEngine->RegisterEnum("ComponentType");
	scriptEngine->RegisterEnumValue("ComponentType", "AudioSource", static_cast<uint>(ComponentType::AudioSource));
	scriptEngine->RegisterEnumValue("ComponentType", "Camera", static_cast<uint>(ComponentType::Camera));
	scriptEngine->RegisterEnumValue("ComponentType", "Collider", static_cast<uint>(ComponentType::Collider));
	scriptEngine->RegisterEnumValue("ComponentType", "Renderable", static_cast<uint>(ComponentType::Renderable));
	scriptEngine->RegisterEnumValue("ComponentType", "Script", static_cast<uint>(ComponentType::Script));
	scriptEngine->RegisterEnumValue("ComponentType", "Transform", static_cast<uint>(ComponentType::Transform));

	// KeyCode
	scriptEngine->RegisterEnum("KeyCode");
	scriptEngine->RegisterEnumValue("KeyCode", "CLICK_LEFT", static_cast<unsigned long>(KeyCode::CLICK_LEFT));
	scriptEngine->RegisterEnumValue("KeyCode", "CLICK_RIGHT", static_cast<unsigned long>(KeyCode::CLICK_RIGHT));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_0", static_cast<unsigned long>(KeyCode::KEY_0));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_1", static_cast<unsigned long>(KeyCode::KEY_1));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_2", static_cast<unsigned long>(KeyCode::KEY_2));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_3", static_cast<unsigned long>(KeyCode::KEY_3));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_4", static_cast<unsigned long>(KeyCode::KEY_4));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_5", static_cast<unsigned long>(KeyCode::KEY_5));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_6", static_cast<unsigned long>(KeyCode::KEY_6));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_7", static_cast<unsigned long>(KeyCode::KEY_7));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_8", static_cast<unsigned long>(KeyCode::KEY_8));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_9", static_cast<unsigned long>(KeyCode::KEY_9));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_A", static_cast<unsigned long>(KeyCode::KEY_A));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_B", static_cast<unsigned long>(KeyCode::KEY_B));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_C", static_cast<unsigned long>(KeyCode::KEY_C));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_D", static_cast<unsigned long>(KeyCode::KEY_D));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_E", static_cast<unsigned long>(KeyCode::KEY_E));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_F", static_cast<unsigned long>(KeyCode::KEY_F));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_G", static_cast<unsigned long>(KeyCode::KEY_G));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_H", static_cast<unsigned long>(KeyCode::KEY_H));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_I", static_cast<unsigned long>(KeyCode::KEY_I));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_J", static_cast<unsigned long>(KeyCode::KEY_J));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_K", static_cast<unsigned long>(KeyCode::KEY_K));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_L", static_cast<unsigned long>(KeyCode::KEY_L));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_M", static_cast<unsigned long>(KeyCode::KEY_M));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_N", static_cast<unsigned long>(KeyCode::KEY_N));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_O", static_cast<unsigned long>(KeyCode::KEY_O));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_P", static_cast<unsigned long>(KeyCode::KEY_P));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_Q", static_cast<unsigned long>(KeyCode::KEY_Q));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_R", static_cast<unsigned long>(KeyCode::KEY_R));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_S", static_cast<unsigned long>(KeyCode::KEY_S));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_T", static_cast<unsigned long>(KeyCode::KEY_T));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_U", static_cast<unsigned long>(KeyCode::KEY_U));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_V", static_cast<unsigned long>(KeyCode::KEY_V));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_W", static_cast<unsigned long>(KeyCode::KEY_W));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_X", static_cast<unsigned long>(KeyCode::KEY_X));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_Y", static_cast<unsigned long>(KeyCode::KEY_Y));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_Z", static_cast<unsigned long>(KeyCode::KEY_Z));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_SHIFT", static_cast<unsigned long>(KeyCode::KEY_SHIFT));
	scriptEngine->RegisterEnumValue("KeyCode", "KEY_CONTROL", static_cast<unsigned long>(KeyCode::KEY_CONTROL));
}

void ScriptInterface::RegisterTypes()
{
	scriptEngine->RegisterInterface("ScriptBehavior");

	scriptEngine->RegisterObjectType("Settings", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Input", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Timer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Renderable", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Material", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptEngine->RegisterObjectType("Vector2", sizeof(Vector2), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
	scriptEngine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
}

void ScriptInterface::RegisterLog()
{
	scriptEngine->RegisterGlobalFunction("void Log(const int& in, const LogType& inType)", asFUNCTIONPR(Log::Write, (const int&, const LogType&), void), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void Log(const bool& in, LogType& inType)", asFUNCTIONPR(Log::Write, (const bool&, const LogType&), void), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void Log(const float& in, LogType& inType)", asFUNCTIONPR(Log::Write, (const float&, const LogType&), void), asCALL_CDECL);
}

void ScriptInterface::RegisterSettings()
{
}

void ScriptInterface::RegisterTimer()
{
	scriptEngine->RegisterGlobalProperty("Timer timer", context->GetSubsystem<Timer>());
	scriptEngine->RegisterObjectMethod("Timer", "const float& GetDeltaTime()", asMETHOD(Timer, GetDeltaTimeSec), asCALL_THISCALL);
}

void ScriptInterface::RegisterInput()
{
	scriptEngine->RegisterGlobalProperty("Input input", context->GetSubsystem<Input>());
	scriptEngine->RegisterObjectMethod("Input", "const Vector2& GetMousePosition()", asMETHOD(Input, GetMousePosition), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Input", "const Vector2& GetMouseMoveValue()", asMETHOD(Input, GetMouseMoveValue), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Input", "const bool KeyPress(const KeyCode& in)", asMETHOD(Input, KeyPress), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Input", "const bool KeyDown(const KeyCode& in)", asMETHOD(Input, KeyDown), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Input", "const bool KeyUp(const KeyCode& in)", asMETHOD(Input, KeyUp), asCALL_THISCALL);
}

void ScriptInterface::RegisterGameObject()
{
	//opAssign - 대입연산자
	scriptEngine->RegisterObjectMethod("GameObject", "GameObject &opAssign(const GameObject &in)", asMETHODPR(GameObject, operator =, (const GameObject&), GameObject&), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "const int& GetID()", asMETHOD(GameObject, GetID), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "const string& GetName()", asMETHOD(GameObject, GetName), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "void SetName(const string& in)", asMETHOD(GameObject, SetName), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "const bool& IsActive()", asMETHOD(GameObject, IsActive), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "void SetIsActive(const bool& in)", asMETHOD(GameObject, SetIsActive), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "Transform &GetTransform()", asMETHOD(GameObject, GetTransform), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "Camera &GetCamera()", asMETHOD(GameObject, GetComponent<Camera>), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("GameObject", "Renderable &GetRenderable()", asMETHOD(GameObject, GetComponent<Renderable>), asCALL_THISCALL);
}

void ScriptInterface::RegisterTransform()
{
	scriptEngine->RegisterObjectMethod("Transform", "Transform &opAssign(const Transform &in)", asMETHODPR(Transform, operator =, (const Transform&), Transform&), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3& GetLocalScale()", asMETHOD(Transform, GetLocalScale), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetLocalScale(const Vector3& in)", asMETHOD(Transform, SetLocalScale), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3& GetLocalRotation()", asMETHOD(Transform, GetLocalRotation), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetLocalRotation(const Vector3& in)", asMETHOD(Transform, SetLocalRotation), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3& GetLocalPosition()", asMETHOD(Transform, GetLocalPosition), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetLocalPosition(const Vector3& in)", asMETHOD(Transform, SetLocalPosition), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetScale()", asMETHOD(Transform, GetScale), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetScale(const Vector3& in)", asMETHOD(Transform, SetScale), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetRotation()", asMETHOD(Transform, GetRotation), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetRotation(const Vector3& in)", asMETHOD(Transform, SetRotation), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetPosition()", asMETHOD(Transform, GetPosition), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "void SetPosition(const Vector3& in)", asMETHOD(Transform, SetPosition), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetUp()", asMETHOD(Transform, GetUp), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetForward()", asMETHOD(Transform, GetForward), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "const Vector3 GetRight()", asMETHOD(Transform, GetRight), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "Transform& GetRoot()", asMETHOD(Transform, GetRoot), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "Transform& GetParent()", asMETHOD(Transform, GetParent), asCALL_THISCALL);
	scriptEngine->RegisterObjectMethod("Transform", "GameObject& GetGameObject()", asMETHOD(Transform, GetGameObject), asCALL_THISCALL);
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

void ConstructorVector2(Vector2* other)
{
	new(other) Vector2(0, 0);
}

void CopyConstructorVector2(const Vector2& in, Vector2* other)
{
	new(other) Vector2(in.x, in.y);
}

void ConstructorVector2Floats(float x, float y, Vector2* other)
{
	new(other) Vector2(x, y);
}

void DestructVector2(Vector2* other)
{
	other->~Vector2();
}

static Vector2& Vector2AddAssignVector2(const Vector2& other, Vector2* self)
{
	return *self = *self + other;
}

void ScriptInterface::RegisterVector2()
{
	scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructorVector2), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(const Vector2 &in)", asFUNCTION(CopyConstructorVector2), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(ConstructorVector2Floats), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructVector2), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opAddAssign(const Vector2 &in)", asFUNCTION(Vector2AddAssignVector2), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opAssign(const Vector2 &in)", asMETHODPR(Vector2, operator=, (const Vector2&), Vector2&), asCALL_THISCALL);
	scriptEngine->RegisterObjectProperty("Vector2", "float x", asOFFSET(Vector2, x));
	scriptEngine->RegisterObjectProperty("Vector2", "float y", asOFFSET(Vector2, y));
}

void ConstructorVector3(Vector3* self)
{
	new(self) Vector3(0, 0, 0);
}

void CopyConstructorVector3(const Vector3& other, Vector3* self)
{
	new(self) Vector3(other.x, other.y, other.z);
}

void ConstructorVector3Floats(float x, float y, float z, Vector3* self)
{
	new(self) Vector3(x, y, z);
}

void DestructVector3(Vector3* self)
{
	self->~Vector3();
}

static Vector3& Vector3Assignment(const Vector3& other, Vector3* self)
{
	return *self = other;
}

static Vector3 Vector3AddVector3(const Vector3& other, Vector3* self)
{
	return *self + other;
}

static Vector3& Vector3AddAssignVector3(const Vector3& other, Vector3* self)
{
	return *self = *self + other;
}

static Vector3& Vector3SubAssignVector3(const Vector3& other, Vector3* self)
{
	return *self = *self - other;
}

static Vector3& Vector3MulAssignVector3(const Vector3& other, Vector3* self)
{
	return *self = *self * other;
}

static Vector3& Vector3MulAssignFloat(float value, Vector3* self)
{
	return *self = *self * value;
}

static Vector3 Vector3MulVector3(const Vector3& other, Vector3* self)
{
	return *self * other;
}

static Vector3 Vector3MulFloat(float value, Vector3* self)
{
	return *self * value;
}

void ScriptInterface::RegisterVector3()
{
	// operator overloads http://www.angelcode.com/angelscript/sdk/docs/manual/doc_script_class_ops.html

	scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructorVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(const Vector3 &in)", asFUNCTION(CopyConstructorVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ConstructorVector3Floats), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opAssign(const Vector3 &in)", asFUNCTION(Vector3Assignment), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opAdd(const Vector3 &in)", asFUNCTION(Vector3AddVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opAddAssign(const Vector3 &in)", asFUNCTION(Vector3AddAssignVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opSubAssign(const Vector3 &in)", asFUNCTION(Vector3SubAssignVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opMulAssign(const Vector3 &in)", asFUNCTION(Vector3MulAssignVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opMulAssign(float)", asFUNCTION(Vector3MulAssignFloat), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opMul(const Vector3 &in)", asFUNCTION(Vector3MulVector3), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opMul(float)", asFUNCTION(Vector3MulFloat), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opMul_r(float)", asFUNCTION(Vector3MulFloat), asCALL_CDECL_OBJLAST);
	scriptEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Vector3, x));
	scriptEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Vector3, y));
	scriptEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Vector3, z));
}
