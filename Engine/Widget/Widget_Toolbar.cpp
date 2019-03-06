#include "stdafx.h"
#include "Widget_Toolbar.h"
#include "./ImGui/Source/imgui_internal.h" //imgui 내부 데이터를 가지고 있음

Widget_Toolbar::Widget_Toolbar(Context * context) : IWidget(context)
{
	title = "ToolBar";
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;
}

Widget_Toolbar::~Widget_Toolbar()
{
}

void Widget_Toolbar::Begin()
{
	ImGuiContext &ctx = *GImGui;
	//Safe Area Padding -> 창의 모서리의 약간의 여유공간
	ctx.NextWindowData.MenuBarOffsetMinVal = ImVec2(ctx.Style.DisplaySafeAreaPadding.x, ImMax(ctx.Style.DisplaySafeAreaPadding.y - ctx.Style.FramePadding.y, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(ctx.Viewports[0]->Pos.x, ctx.Viewports[0]->Pos.y + 23.9f));
	ImGui::SetNextWindowSize(ImVec2(ctx.Viewports[0]->Size.x, ctx.NextWindowData.MenuBarOffsetMinVal.y + ctx.FontBaseSize + ctx.Style.FramePadding.y + 20.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 5.0f));
	ImGui::Begin(title.c_str(), &bVisible, windowFlags);
}

void Widget_Toolbar::Render()
{
	ImGui::SameLine(); //같은 줄에 렌더
	ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[Engine::IsOnEngineFlags(EngineFlags_Game) ? ImGuiCol_ButtonActive : ImGuiCol_Button]);

	if (IconProvider::Get().ImageButton(IconType::Button_Play, 20.0f)) Engine::ToggleEngineFlags(EngineFlags_Game);

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
