#include "stdafx.h"
#include "Widget_Scene.h"
#include "./Helper/EditorHelper.h"

Widget_Scene::Widget_Scene(Context * context)
	: IWidget(context)
{
	title = "Scene";
	windowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	renderer = context->GetSubsystem<Renderer>();
	sceneMgr = context->GetSubsystem<SceneManager>();
}

Widget_Scene::~Widget_Scene()
{
}

void Widget_Scene::Render()
{
	ShowFrame();
}

void Widget_Scene::ShowFrame()
{
	Vector2 framePos = EditorHelper::ToVector2(ImGui::GetCursorPos() + ImGui::GetWindowPos());
	Vector2 frameSize = EditorHelper::ToVector2(ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin());

	frameSize.x -= static_cast<float>((static_cast<int>(frameSize.x) % 2 != 0) ? 1 : 0);
	frameSize.y -= static_cast<float>((static_cast<int>(frameSize.y) % 2 != 0) ? 1 : 0);

	ImGui::Image(renderer->GetFrameResourceView(), EditorHelper::ToImVec2(frameSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(50, 127, 166, 255));
}

