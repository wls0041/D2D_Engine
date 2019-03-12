#include "stdafx.h"
#include "Widget_Inspector.h"
#include "./Scene/GameObject.h"
#include "./Scene/Component/Transform.h"
#include "./Scene/Component/Camera.h"

Widget_Inspector::Widget_Inspector(Context * context)
	: IWidget(context)
{
	title = "Inspector";
}

void Widget_Inspector::Render()
{
	if (!bVisible) return;

	if (EditorHelper::CurrentObject) {
		ShowTransform(EditorHelper::CurrentObject->GetTransform());
		ShowCamera(EditorHelper::CurrentObject->GetComponent<Camera>());
	}
}

void Widget_Inspector::ShowTransform(Transform * transform)
{
	if (!transform) return;

	Vector3 scale = transform->GetScale();
	Vector3 rotation = transform->GetRotation();
	Vector3 position = transform->GetPosition();

	auto ShowFloat = [](const char *label, float *value) {
		float step = 1.0f;
		float step_fast = 1.0f;
		char *decimals = const_cast<char*>("%.3f");
		auto flag = ImGuiInputTextFlags_CharsDecimal;

		ImGui::PushItemWidth(100.0f);
		ImGui::InputFloat(label, value, step, step_fast, decimals, flag);
		ImGui::PopItemWidth();
	};

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Pos");
		ImGui::SameLine(70.0f);	ShowFloat("##PosX", &position.x);
		ImGui::SameLine();		ShowFloat("##PosY", &position.y);
		ImGui::SameLine();		ShowFloat("##PosZ", &position.z);

		ImGui::Text("Rot");
		ImGui::SameLine(70.0f);	ShowFloat("##RotX", &rotation.x);
		ImGui::SameLine();		ShowFloat("##RotY", &rotation.y);
		ImGui::SameLine();		ShowFloat("##RotZ", &rotation.z);

		ImGui::Text("Sca");
		ImGui::SameLine(70.0f);	ShowFloat("##ScaX", &scale.x);
		ImGui::SameLine();  	ShowFloat("##ScaY", &scale.y);
		ImGui::SameLine();	    ShowFloat("##ScaZ", &scale.z);
	}

	transform->SetScale(scale);
	transform->SetRotation(rotation);
	transform->SetPosition(position);
}

void Widget_Inspector::ShowCamera(Camera * camera)
{
}
