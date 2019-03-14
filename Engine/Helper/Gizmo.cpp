#include "stdafx.h"
#include "Gizmo.h"
#include "./ImGui/ImGuizmo.h"
#include "./Scene/Component/Camera.h"
#include "./Scene/Component/Transform.h"

void Gizmo::TransformGizmo(Camera * camera, Transform * transform, const Vector2 & framePos, const Vector2 & frameSize)
{
	if (!camera || !transform) return;

	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mode(ImGuizmo::WORLD);

	if (ImGui::IsKeyPressed(87)) //w
		operation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69)) //e
		operation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) //r
		operation = ImGuizmo::SCALE;

	auto view = camera->GetViewMatrix().Transpose();
	auto proj = camera->GetProjectionMatrix().Transpose();
	auto matrix = transform->GetWorldMatrix().Transpose(); //imgui는 행우선

	ImGuizmo::SetOrthographic(true);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(framePos.x, framePos.y, frameSize.x, frameSize.y);
	ImGuizmo::Manipulate(view, proj, operation, mode, matrix);

	matrix.Transpose();
	transform->SetScale(matrix.GetScale());
	transform->SetRotation(matrix.GetRotation());
	transform->SetPosition(matrix.GetTranslation());
}
