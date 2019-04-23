#include "stdafx.h"
#include "Widget_Toolbar.h"
#include "./ImGui/Source/imgui_internal.h" //imgui ���� �����͸� ������ ����

Widget_Toolbar::Widget_Toolbar(Context * context) : IWidget(context), rendererOptionAlpha(1.0f), bShowRendrerOption(false)
{
	title = "ToolBar";
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;

	renderer = context->GetSubsystem<Renderer>();
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
}

void Widget_Toolbar::Render()
{
	//Play Button
	ImGui::SameLine(); //���� �ٿ� ����
	ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[bShowRendrerOption ? ImGuiCol_ButtonActive : ImGuiCol_Button]);

	if (IconProvider::Get().ImageButton(IconType::Button_Play_Big, 20.0f)) Engine::ToggleEngineFlags(EngineFlags_Game);

	ImGui::PopStyleColor();	
	
	//Option Button
	ImGui::SameLine(); //���� �ٿ� ����
	ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[Engine::IsOnEngineFlags(EngineFlags_Game) ? ImGuiCol_ButtonActive : ImGuiCol_Button]);

	if (IconProvider::Get().ImageButton(IconType::Button_Option, 20.0f)) bShowRendrerOption = true;

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	if (bShowRendrerOption) ShowRendererOptions();
}

void Widget_Toolbar::ShowRendererOptions()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, rendererOptionAlpha);

	ImGui::Begin("Rendrer Options", &bShowRendrerOption, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking);
	{
		ImGui::TextUnformatted("Opacity");
		ImGui::SameLine();
		ImGui::SliderFloat("##Opacity", &rendererOptionAlpha, 0.1f, 1.0f);
	}
	ImGui::End();

	ImGui::PopStyleVar();
}
