#pragma once
#include "../ImGui/Source/imgui.h"

class EditorHelper
{
public:
	static class GameObject *CurrentObject;

public:
	static const ImVec2 ToImVec2(const Vector2 &rhs) { return ImVec2(rhs.x, rhs.y); }
	static const ImVec4 ToImVec3(const Vector3 &rhs) { return ImVec4(rhs.x, rhs.y, rhs.z, 1.0f); }

	static const Vector2 ToVector2(const ImVec2 &rhs) { return Vector2(rhs.x, rhs.y); }
	static const Vector3 ToVector3(const ImVec4 &rhs) { return Vector3(rhs.x, rhs.y, rhs.z); }

public:
	static EditorHelper &Get() { 
		static EditorHelper instance; 
		return instance;
	}

	void Initialize(class Context *context);

	auto GetSelectScene() const -> class Scene* { return selectScene; }
	void SetSelectScene(class Scene *scene) { this->selectScene = scene; }

	auto GetSelectObject() const -> class GameObject* { return selectObject; }
	void SetSelectObject(class GameObject *object) { this->selectObject = object; }

private:
	EditorHelper() = default;
	~EditorHelper() = default;
	
	EditorHelper(const EditorHelper&) = delete;
	EditorHelper &operator=(const EditorHelper&) = delete;
	
private:
	class Context *context = nullptr;
	class ResourceManager *resourceManager = nullptr;
	class SceneManager *sceneManager = nullptr;
	class Renderer *renderer = nullptr;
	class Scene *selectScene = nullptr;
	class GameObject *selectObject = nullptr;
};

static inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }