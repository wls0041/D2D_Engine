#include "stdafx.h"
#include "Widget_Hierarchy.h"
#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"
#include "./Scene/Component/Renderable.h"
#include "./Scene/Component/Transform.h"
#include "./Scene/Component/ParticleEmitter.h"

Widget_Hierarchy::Widget_Hierarchy(Context * context)
	: IWidget(context)
{
	title = "Hierarchy";
	sceneManager = context->GetSubsystem<SceneManager>();
}

Widget_Hierarchy::~Widget_Hierarchy()
{
}

void Widget_Hierarchy::Render()
{
	if (!bVisible) return;

	ShowHierarchy();
	if (ImGui::IsWindowHovered()) { //윈도우 위에 마우스가 덮여 있을 때
		if (ImGui::GetIO().MouseDown[1]) ImGui::OpenPopup("Hierarchy MenuPopup");
	}
	ShowPopup();
}

void Widget_Hierarchy::ShowHierarchy()
{
	auto scenes = sceneManager->GetScenes();
	
	for (auto scene : scenes) AddScene(scene.second);
}

void Widget_Hierarchy::AddScene(Scene * scene)
{
	if (!scene) return;
	auto objects = scene->GetObjects();

	if (ImGui::CollapsingHeader(scene->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {  //collapsing은 end(pop)필요 없음
		for (auto object : objects) { 
			AddObject(object); 

			if (ImGui::IsItemClicked()) EditorHelper::Get().SetSelectObject(object);
		}
	}
}

void Widget_Hierarchy::AddObject(GameObject * object)
{
	if (!object) return;
	if (ImGui::TreeNodeEx(object->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) { ImGui::TreePop(); }
}

void Widget_Hierarchy::ShowPopup()
{
	MenuPopup();
}

void Widget_Hierarchy::MenuPopup()
{
	if (ImGui::BeginPopup("Hierarchy MenuPopup")) {
		if (ImGui::MenuItem("Copy")) {}
		if (ImGui::MenuItem("Delete")) {}

		ImGui::Separator();

		if (ImGui::MenuItem("Empty Object")) {}
		if (ImGui::BeginMenu("Create 2D")) {
			if (ImGui::MenuItem("Quad")) CreateQuad();
			if (ImGui::MenuItem("ScreenQuad")) CreateScreenQuad();

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}

GameObject * Widget_Hierarchy::CreateEmpty()
{
	auto scene = sceneManager->GetCurrentScene();

	if (scene) {
		auto object = scene->CreateObject();
		object->SetName("Object");
		return object;
	}
	return nullptr;
}

void Widget_Hierarchy::CreateQuad()
{
	auto object = CreateEmpty();

	if (object) {
		auto renderable = object->AddComponent<Renderable>();
		renderable->SetStandardMaterial();
		renderable->SetStandardMesh(MeshType::Quad);
		object->SetName("Quad");

		object->GetTransform()->SetScale({ 50.0f, 50.0f, 1.0f });

		////임시
		object->AddComponent<ParticleEmitter>();
	}
}

void Widget_Hierarchy::CreateScreenQuad()
{
	auto object = CreateEmpty();

	if (object) {
		auto renderable = object->AddComponent<Renderable>();
		renderable->SetStandardMaterial();
		renderable->SetStandardMesh(MeshType::ScreenQuad);
		object->SetName("ScreenQuad");
	}
}
