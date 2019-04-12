#include "stdafx.h"
#include "Widget_Assets.h"

Widget_Assets::Widget_Assets(Context * context) : IWidget(context), itemSize(100.0f), itemSizeMin(50.0f), itemSizeMax(200.0f)
{
	resourceMgr = context->GetSubsystem<ResourceManager>();

	title = "Assets";
	currentPath = "../../_Assets/Texture/";

	UpdateItems(currentPath);
}

void Widget_Assets::Render()
{
	if (!bVisible) return;

	if (ImGui::Button("Directory")) {
		//TODO :
	}

	ImGui::SameLine();
	ImGui::Text(currentPath.c_str());

	ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * 0.8f);
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() * 0.207f);
	{
		ImGui::SliderFloat("##ItemSize", &itemSize, itemSizeMin, itemSizeMax); //1, 바꿀값, 최소값, 최대값, 포맷, 한번에 바뀌는 크기
	}
	ImGui::Separator();

	ShowItem();
}

void Widget_Assets::ShowItem()
{
	auto PushStyle = []() {
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 18.0f));
	};

	auto PopStyle = []() { ImGui::PopStyleVar(2); };
	auto contentSize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();

	PushStyle();
	{
		ImGui::BeginChild("##Child", contentSize, true);
		{
			int columns = static_cast<int>(ImGui::GetWindowContentRegionWidth() / itemSize);
			columns = columns < 1 ? 1 : columns;
			
			ImGui::Columns(columns, nullptr, true);
			for (const auto &item : items) {
				//ImageButton --> 버튼 외곽의 색 삭제
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); //외곽선 색 없앰 
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); //버튼 색 없앰    

				if (IconProvider::Get().ImageButton(item.thumbnail, ImVec2(itemSize, itemSize - 23.0f))) {
					//TODO :
				}
				ImGui::PopStyleColor(2);

				//DragDrop
				DragDrop::SetDragDropPayload(DragDropPayloadType::Texture, item.label);
				
				//Label
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - itemSize);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + itemSize - 13.0f);
				ImGui::PushItemWidth(itemSize + 8.5f); //pop이 되기 전까지 다음에 오는 모든 컨텐츠의 사이즈를 이것으로 고정
				{
					ImGui::TextWrapped(item.label.c_str()); //크기가 넘어가면 줄바꿈해줌
				}
				ImGui::PopItemWidth();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + itemSize);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - itemSize + 13.0f); //커서 복귀

				ImGui::NextColumn();
			}
		}
		ImGui::EndChild();
	}
	PopStyle();

}

void Widget_Assets::UpdateItems(const std::string & path)
{
	if (!FileSystem::IsDirectory(path)) return;

	items.clear();
	items.shrink_to_fit();

	auto files = FileSystem::GetFilesInDirectory(path);
	for (const auto &file : files) items.emplace_back(file, *(IconProvider::Get().Load(file)));

}
