#pragma once
#include "stdafx.h"

enum class DragDropPayloadType : uint { Unknown, Texture, Object, Audio, Script };

//드래그 -> 이벤트 발생 -> 해당 정보를 임시 저장소에 저장
//드롭 -> 이벤트 발생 -> 저장해둔 정보를 가져옴

class DragDrop final
{
public:
	static void SetDragDropPayload(const DragDropPayloadType &type, const std::string &data) {
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload(reinterpret_cast<const char*>(&type), data.c_str(), data.length() + 1); // + 1 == null문자
			ImGui::Text(data.c_str());
			ImGui::EndDragDropSource();
		}
	}

	static const std::string GetDragDropPayload(const DragDropPayloadType&type) {
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(reinterpret_cast<const char*>(&type));

			if (payload) return reinterpret_cast<const char*>(payload->Data);

			ImGui::EndDragDropTarget(); //return해도 괜찮음.... 왜?
		}
		return "";
	}

};