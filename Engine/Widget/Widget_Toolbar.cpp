#include "stdafx.h"
#include "Widget_Toolbar.h"
#include "./ImGui/Source/imgui_internal.h" //imgui ���� �����͸� ������ ����

Widget_Toolbar::Widget_Toolbar(Context * context) : IWidget(context)
{
	title = "ToolBar";
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
}

Widget_Toolbar::~Widget_Toolbar()
{
}

void Widget_Toolbar::Begin()
{
	ImGuiContext &ctx = *GImGui;
	//Safe Area Padding -> â�� �𼭸��� �ణ�� ��������
	ctx.NextWindowData.MenuBarOffsetMinVal = ImVec2(ctx.Style.DisplaySafeAreaPadding.x, ImMax(ctx.Style.DisplaySafeAreaPadding.y - ctx.Style.FramePadding.y, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(ctx.Viewports[0]->Pos.x, ctx.Viewports[0]->Pos.y + 23.9f));
	ImGui::SetNextWindowSize(ImVec2(ctx.Viewports[0]->Size.x, ctx.NextWindowData.MenuBarOffsetMinVal.y + ctx.FontBaseSize + ctx.Style.FramePadding.y + 20.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 5.0f));
	ImGui::Begin(title.c_str(), &bVisible, windowFlags);
	ImGui::PopStyleVar();
}

void Widget_Toolbar::Render()
{
}