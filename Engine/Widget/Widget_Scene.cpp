#include "stdafx.h"
#include "Widget_Scene.h"
#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"
#include "./Scene/Component/Camera.h"
#include "./Scene/Component/Transform.h"
#include "./Scene/Component/Renderable.h"

Widget_Scene::Widget_Scene(Context * context)
	: IWidget(context), camera(nullptr), framePos(0.0f), frameSize(0.0f)
{
	title = "Scene";
	windowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	renderer = context->GetSubsystem<Renderer>();
	sceneMgr = context->GetSubsystem<SceneManager>();
}

void Widget_Scene::Render()
{
	ShowFrame();
	ShowGizmo();
	Picking();
}

void Widget_Scene::ShowFrame()
{
	framePos = EditorHelper::ToVector2(ImGui::GetCursorPos() + ImGui::GetWindowPos());
	frameSize = EditorHelper::ToVector2(ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin());

	frameSize.x -= static_cast<float>((static_cast<int>(frameSize.x) % 2 != 0) ? 1 : 0);
	frameSize.y -= static_cast<float>((static_cast<int>(frameSize.y) % 2 != 0) ? 1 : 0);

	ImGui::Image(renderer->GetFrameResourceView(), EditorHelper::ToImVec2(frameSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(255, 0, 0, 255));

	camera = renderer->GetMainCamera();
}

void Widget_Scene::ShowGizmo()
{
	if (!EditorHelper::CurrentObject || !camera) return;

	Gizmo::TransformGizmo(camera, EditorHelper::CurrentObject->GetTransform(), framePos, frameSize);
}

void Widget_Scene::Picking()
{
	bool bCheck = false;
	bCheck |= !camera;
	bCheck |= !ImGui::IsMouseClicked(0);
	bCheck |= !ImGui::IsWindowHovered(
		ImGuiHoveredFlags_AllowWhenBlockedByPopup | //팝업창이 떠 있어도 클릭 가능
		ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
	);
	if (bCheck) return;

	Vector2 mousePos = EditorHelper::ToVector2(ImGui::GetMousePos());
	Vector3 worldPos = camera->ScreenToWorldPoint(mousePos); //unprojection
	
	auto objects = sceneMgr->GetCurrentScene()->GetObjects();
	for (auto &object : objects) {
		if (!object->HasComponent<Renderable>()) continue;

		auto renderable = object->GetComponent<Renderable>();
		auto transform = object->GetTransform();
		auto boundBox = renderable->GetBoundBox();

		boundBox = BoundBox::Transformed(boundBox, transform->GetWorldMatrix());
		auto result = boundBox.IsInside(worldPos);

		if (result == Intersection::Inside) EditorHelper::CurrentObject = object;

	}
}

