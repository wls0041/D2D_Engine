#include "stdafx.h"
#include "Widget_Inspector.h"
#include "./Scene/GameObject.h"
#include "./Scene/Component/Transform.h"
#include "./Scene/Component/Camera.h"
#include "./Scene/Component/Renderable.h"
#include "./Scene/Component/Script.h"
#include "./Scene/Component/Collider.h"
#include "./Scene/Component/Light.h"
#include "./Scene/Component/Tilemap.h"

Widget_Inspector::Widget_Inspector(Context * context)
	: IWidget(context)
{
	title = "Inspector";
}

void Widget_Inspector::Render()
{
	if (!bVisible) return;

	if (auto object = EditorHelper::Get().GetSelectObject()) {
		ShowTransform(object->GetTransform());
		ShowCamera(object->GetComponent<Camera>());
		ShowRenderable(object->GetComponent<Renderable>());
		ShowScript(object->GetComponent<Script>());
		ShowLight(object->GetComponent<Light>());
		ShowTilemap(object->GetComponent<Tilemap>());
	}

	ShowAddComponent();
	ShowComponentPopup();
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
	if (!camera) return;
	 
	auto projectionType = camera->GetProjectionType();
	const char *projectionNames[] = { "Perspective", "Orthographic" };
	const char *projectionName = projectionNames[static_cast<uint>(projectionType)];

	auto zoom = camera->GetZoom();
	auto nearPlane = camera->GetNearPlane();
	auto farPlane = camera->GetFarPlane();

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::PushItemWidth(110.0f); 
		{
			ImGui::Text("Projection");
			ImGui::SameLine(140.0f);

			if (ImGui::BeginCombo("##ProjectionType", projectionName)) {
				for (uint i = 0; i < IM_ARRAYSIZE(projectionNames); i++) {
					bool bSelected = projectionName == projectionNames[i];
					if (ImGui::Selectable(projectionNames[i], bSelected)) projectionName = projectionNames[i];
					if (bSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Text("Zoom");
			ImGui::SameLine(140.0f);
			ImGui::SliderFloat("##Zoom", &zoom, 0.1f, 100.0f, "%.3f", 0.1f);
			
			ImGui::Text("Near Plane");
			ImGui::SameLine(140.0f);
			ImGui::InputFloat("##Near", &nearPlane, 0.1f, 0.1f, "%.3f", ImGuiInputTextFlags_CharsDecimal);

			ImGui::Text("Far Plane");
			ImGui::SameLine(140.0f);
			ImGui::InputFloat("##Far", &farPlane, 0.1f, 0.1f, "%.3f", ImGuiInputTextFlags_CharsDecimal);

		}
		ImGui::PopItemWidth();
	}

	camera->SetProjectionType(projectionType);
	camera->SetZoom(zoom);
	camera->SetNearPlane(nearPlane);
	camera->SetFarPlane(farPlane);
}

void Widget_Inspector::ShowRenderable(Renderable * renderable)
{
	if (!renderable) return;
	if (ImGui::CollapsingHeader("Renderable", ImGuiTreeNodeFlags_DefaultOpen)) {

	}
}

void Widget_Inspector::ShowScript(Script * script)
{
	if (!script) return;
	if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen)) {
		std::string scriptName = script->GetName();

		ImGui::Text("Script");
		ImGui::SameLine(140.0f);
		ImGui::PushItemWidth(110.0f);
		{
			ImGui::InputText("##ScriptNAme", &scriptName, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::Button("Edit")) {
				Tool_Script::Get().SetScript(script->GetScriptPath());
				Tool_Script::Get().SetIsVisible(true);
			}
		}
		ImGui::PopItemWidth();
	}
}

void Widget_Inspector::ShowLight(Light * light)
{
	if (!light) return;
	auto GetColor = [](const Color &color)->Vector3 {
		return Vector3(color.r, color.g, color.b);
	};

	Vector3 color = GetColor(light->GetColor());
	float intensity = light->GetColor().a;
	float minDistance = light->GetMinDistance();
	float maxDistance = light->GetMaxDistance();
	float distanceFactor = light->GetDistanceFactor();
	bool bTwinkle = light->IsTwinkle();

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Color");
		ImGui::SameLine(70.0f);
		ImGui::ColorEdit3("##Color", color);
	
		ImGui::Text("Intensity");
		ImGui::SameLine(70.0f);
		ImGui::SliderFloat("##Intensity", &intensity, 0.0f, 1.0f);

		ImGui::Text("MinDist");
		ImGui::SameLine(70.0f);
		ImGui::DragFloat("##MinDist", &minDistance);

		ImGui::Text("MaxDist");
		ImGui::SameLine(70.0f);
		ImGui::DragFloat("##MaxDist", &maxDistance);

		ImGui::Text("DistFactor");
		ImGui::SameLine(70.0f);
		ImGui::DragFloat("##DistFactor", &distanceFactor);

		ImGui::Text("Twinkle");
		ImGui::SameLine(70.0f);
		ImGui::Checkbox("##Twinkle", &bTwinkle);
	}

	light->SetColor(color);
	light->SetIntensity(intensity);
	light->SetMinDistance(minDistance);
	light->SetMaxDistance(maxDistance);
	light->SetDistanceFactor(distanceFactor);
	light->SetIsTwinkle(bTwinkle);
}

void Widget_Inspector::ShowTilemap(Tilemap * tilemap)
{
	if (!tilemap) return;

	static auto tileset_grid_color = Color(0, 1, 0, 1);
	static auto bTileset_grid = true;
	static auto bShowTileset = false;
	static auto w = tilemap->GetHeight();
	static auto h = tilemap->GetHeight();
	static auto s = tilemap->GetSpacing();

	if (ImGui::CollapsingHeader("Tilemap", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Show Grid", &bTileset_grid);
		ImGui::Checkbox("Show Tileset", &bShowTileset);
		ImGui::ColorEdit4("Color", tileset_grid_color);

		ImGui::InputInt("Width", reinterpret_cast<int*>(&w));
		ImGui::InputInt("Height", reinterpret_cast<int*>(&h));
		ImGui::InputInt("Spacing", reinterpret_cast<int*>(&s));

		if (ImGui::Button("Create Tilemap")) {
			tilemap->ClearTilemap();
			tilemap->CreateTilemap(w, h, s);
		}
	}

	if (!bShowTileset) return;
	if (ImGui::Begin("Tileset", &bShowTileset, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginTabBar("Tileset")) {
			auto tilesets = tilemap->GetTileSets();
			auto spacing = tilemap->GetSpacing();
			auto drawList = ImGui::GetWindowDrawList();
			auto windowPos = ImGui::GetCursorPos() + ImGui::GetWindowPos();

			windowPos.x -= ImGui::GetScrollX();
			windowPos.y -= ImGui::GetScrollY();

			int count = 0;
			for (const auto &tileset : tilesets) {
				auto width = tileset->GetWidth();
				auto height = tileset->GetHeight();
				
				if (ImGui::BeginTabItem(tileset->GetResourceName().c_str())) {
					ImGui::Image(
						tileset->GetShaderResourceView(),
						ImVec2(static_cast<float>(width), static_cast<float>(height))
					);

					if (bTileset_grid) {
						//vertical
						for (uint x = 0; x <= width; x += spacing) 
							drawList->AddLine(ImVec2(x + windowPos.x, windowPos.y), ImVec2(x + windowPos.x, windowPos.y + height), tileset_grid_color);

						//horizontal
						for (uint y = 0; y <= height; y += spacing) 
							drawList->AddLine(ImVec2(windowPos.x, y + windowPos.y), ImVec2(windowPos.x + width, y + windowPos.y), tileset_grid_color);

					}
					
					if (ImGui::IsWindowHovered()) {
						if (ImGui::IsMouseClicked(0)) {
							auto mousePos = ImGui::GetMousePos() - windowPos;
							auto temp = ImVec2(mousePos.x + ImGui::GetScrollX(), mousePos.y + ImGui::GetScrollY());

							uint x = static_cast<uint>(temp.x / spacing);
							uint y = static_cast<uint>(temp.x / spacing);

							TileData data;
							data.SpriteOffset = Vector2(static_cast<float>(x * spacing), static_cast<float>(y * spacing));
							data.SpriteSize = Vector2(spacing);
							data.TextureSize = Vector2(static_cast<float>(tileset->GetWidth()), static_cast<float>(tileset->GetHeight()));
							data.TilesetIndex = count;

							tilemap->SetCurrentTileData(data);
						}
					}
					ImGui::EndTabItem();
				}
				count++;
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void Widget_Inspector::ShowAddComponent()
{
	ImGui::Separator();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 50.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

	if (ImGui::Button("Add Component")) {
		ImGui::OpenPopup("##Component Popup");
	}
}

void Widget_Inspector::ShowComponentPopup()
{
	if (auto object = EditorHelper::Get().GetSelectObject()) {
		if (ImGui::BeginPopup("##Component Popup")) {
			if (ImGui::MenuItem("Camera")) object->AddComponent<Camera>();
			if (ImGui::MenuItem("Renderable")) object->AddComponent<Renderable>();
			if (ImGui::MenuItem("Script")) object->AddComponent<Script>();
			if (ImGui::MenuItem("Collider")) object->AddComponent<Collider>();
			if (ImGui::MenuItem("Light")) {}

			ImGui::EndPopup();
		}
	}
}
