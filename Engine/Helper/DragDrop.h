#pragma once
#include "stdafx.h"

enum class DragDropPayloadType : uint { Unknown, Texture, Object, Audio, Script };

//�巡�� -> �̺�Ʈ �߻� -> �ش� ������ �ӽ� ����ҿ� ����
//��� -> �̺�Ʈ �߻� -> �����ص� ������ ������

class DragDrop final
{
public:
	static void SetDragDropPayload(const DragDropPayloadType &type, const std::string &data) {
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload(reinterpret_cast<const char*>(&type), data.c_str(), data.length() + 1); // + 1 == null����
			ImGui::Text(data.c_str());
			ImGui::EndDragDropSource();
		}
	}

	static const std::string GetDragDropPayload(const DragDropPayloadType&type) {
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(reinterpret_cast<const char*>(&type));

			if (payload) return reinterpret_cast<const char*>(payload->Data);

			ImGui::EndDragDropTarget(); //return�ص� ������.... ��?
		}
		return "";
	}

};