#include "stdafx.h"
#include "Tool_Sprite.h"
#include "./ImGui/Source/imgui.h"

void Tool_Sprite::Initialize(Context * context)
{
	this->context = context;
	this->resourceMgr = context->GetSubsystem<ResourceManager>();
	this->texture = nullptr;
	this->gridSpacing = 16;
	this->gridColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
	this->bGrid = true;
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
		//DragDropEvent(); //위치는 원하는 것 바로 밑에 먹여야 함

		ShowSpriteEdit(size);
	}
	ImGui::End();
}

void Tool_Sprite::ShowSpriteFrame(const ImVec2 & size)
{
	ImGui::BeginChild("##SpriteFrame", ImVec2(size.x * 0.5f, 0.0f), true, ImGuiWindowFlags_HorizontalScrollbar);
	{
		if (texture) {
			if (bGrid) {
				ImVec2 windowPos = ImGui::GetCursorPos() + ImGui::GetWindowPos();
				ImDrawList *drawList = ImGui::GetWindowDrawList(); //imgui에서 무엇인가를 그릴 때 필요함

				//Vertical
				for (uint x = 0; x < static_cast<uint>(texture->GetSize().x); x += gridSpacing) {
					drawList->AddLine(ImVec2(x + windowPos.x, windowPos.y), ImVec2(x + windowPos.x, windowPos.y + texture->GetSize().y), gridColor);
				}

				//Horizontal
				for (uint y = 0; y < static_cast<uint>(texture->GetSize().y); y += gridSpacing) {
					drawList->AddLine(ImVec2(windowPos.x, y + windowPos.y), ImVec2(windowPos.x + texture->GetSize().x, y + windowPos.y), gridColor);
				}
				ImGui::Image(texture->GetShaderResourceView(), EditorHelper::ToImVec2(texture->GetSize()));
			}
		}
	}
	ImGui::EndChild();
	DragDropEvent();
}

void Tool_Sprite::ShowSpriteEdit(const ImVec2 & size)
{
	ImGui::SameLine();
	ImGui::BeginChild("##SpriteEdit", ImVec2(size.x * 0.5f - 20.0f, 0.0f), true);
	{
		ImGui::Text("Grid");
		ImGui::SameLine();
		ImGui::Checkbox("##Is Draw", &bGrid);

		ImGui::SameLine();
		ImGui::ColorEdit4("##Grid Color", gridColor);
	}
	ImGui::EndChild();
}

void Tool_Sprite::DragDropEvent()
{
	auto data = DragDrop::GetDragDropPayload(DragDropPayloadType::Texture);
	if (data.length()) {
		texture = resourceMgr->Load<Texture>(data);
	}
}

