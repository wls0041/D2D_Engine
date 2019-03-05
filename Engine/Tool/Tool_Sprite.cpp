#include "stdafx.h"
#include "Tool_Sprite.h"
#include "./ImGui/Source/imgui.h"

void Tool_Sprite::Initialize(Context * context)
{
	this->context = context;
	this->resourceMgr = context->GetSubsystem<ResourceManager>();
	this->texture = nullptr;
	this->bVisible = false;
}

void Tool_Sprite::Render()
{
	if (!bVisible) return;

	ImGui::Begin("SpriteTool", &bVisible);
	{
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		ImVec2 size = ImGui::GetWindowSize();
		ShowSpriteFrame(size);
	}
	ImGui::End();
}

void Tool_Sprite::ShowSpriteFrame(const ImVec2 & size)
{
}
