#include "stdafx.h"
#include "Widget_Scene.h"
#include "../ImGui/ImGuizmo.h"
#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"
#include "./Scene/Component/Camera.h"
#include "./Scene/Component/Transform.h"
#include "./Scene/Component/Renderable.h"
#include "./Scene/Component/Tilemap.h"

Widget_Scene::Widget_Scene(Context * context)
	: IWidget(context), framePos(0.0f), frameSize(0.0f)
{
	title = "Scene";
	windowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	renderer = context->GetSubsystem<Renderer>();
	sceneMgr = context->GetSubsystem<SceneManager>();
}

void Widget_Scene::Render()
{
	if (!bVisible) return;

	ShowFrame();
	ShowGizmo();
	Picking();
	DragDropEvent();
}

void Widget_Scene::ShowFrame()
{
	framePos = EditorHelper::ToVector2(ImGui::GetCursorPos() + ImGui::GetWindowPos());
	frameSize = EditorHelper::ToVector2(ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin());

	frameSize.x -= static_cast<float>((static_cast<int>(frameSize.x) % 2 != 0) ? 1 : 0);
	frameSize.y -= static_cast<float>((static_cast<int>(frameSize.y) % 2 != 0) ? 1 : 0);

	ImGui::Image(renderer->GetFrameResourceView(), EditorHelper::ToImVec2(frameSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(255, 0, 0, 255));

	if (Settings::Get().GetRelative() != frameSize) Settings::Get().SetRelative(frameSize);
}

void Widget_Scene::ShowGizmo()
{
	if (!EditorHelper::Get().GetSelectObject()) return;

	auto camera = renderer->GetMainCamera();
	auto transform = EditorHelper::Get().GetSelectObject()->GetTransform();

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

void Widget_Scene::Picking()
{
	auto camera = renderer->GetMainCamera();

	bool bCheck = false;
	bCheck |= !camera;
	bCheck |= !ImGui::IsMouseClicked(0);
	bCheck |= !ImGui::IsWindowHovered(
		ImGuiHoveredFlags_AllowWhenBlockedByPopup | //팝업창이 떠 있어도 클릭 가능
		ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
	);
	if (bCheck) return;

	Vector2 mousePos = EditorHelper::ToVector2(ImGui::GetMousePos()) - framePos;
	Vector3 worldPos = camera->ScreenToWorldPoint(mousePos); //unprojection
	
	auto objects = sceneMgr->GetCurrentScene()->GetObjects();
	for (auto &object : objects) {
		if (auto tilemap = object->GetComponent<Tilemap>()) {
			worldPos -= tilemap->GetAdjustPosition();

			if (worldPos.x > 0 && worldPos.y > 0 && worldPos.x < tilemap->GetWidth() && worldPos.y < tilemap->GetHeight()) {
				uint x = static_cast<uint>(worldPos.x / tilemap->GetSpacing());
				uint y = static_cast<uint>(worldPos.y / tilemap->GetSpacing());

				tilemap->SetTile(x, y);
			}
		}
		if (!object->HasComponent<Renderable>()) continue;

		auto renderable = object->GetComponent<Renderable>();
		auto transform = object->GetTransform();
		auto boundBox = renderable->GetBoundBox();

		auto result = boundBox.IsInside(worldPos);

		if (result == Intersection::Inside) EditorHelper::Get().SetSelectObject(object);

	}
}

void Widget_Scene::DragDropEvent()
{
	auto data = DragDrop::GetDragDropPayload(DragDropPayloadType::Texture);
	if (data.length()) {
		if (auto object = EditorHelper::Get().GetSelectObject()) {
			auto renderable = object->GetComponent<Renderable>();
			auto material = renderable->GetMaterial();

			material->SetDiffuseTexture(data);
		}
	}
}

