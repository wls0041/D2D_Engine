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
};

static inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }