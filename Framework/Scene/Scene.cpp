#include "Framework.h"
#include "Scene.h"
#include "GameObject.h"
#include "./Component/Transform.h"
#include "./Component/Camera.h"
#include "./Component/Script.h"

Scene::Scene(Context * context)
	: context(context), name(""), bEditorMode(true), bAdded(false), sceneCamera(nullptr)
{
	renderer = context->GetSubsystem<Renderer>();
	sceneCamera = new GameObject(context);
	sceneCamera->Initialize(sceneCamera->AddComponent<Transform>()); //component 1
	sceneCamera->AddComponent<Camera>(); //component 2
	sceneCamera->SetName("SceneCamera");

	//test
	auto script = sceneCamera->AddComponent<Script>();
	script->SetScript("../../_Assts/Script/Move.as");

	AddObject(sceneCamera);
}

Scene::~Scene()
{
	for (auto object : objects) SAFE_DELETE(object);
	objects.clear();
	objects.shrink_to_fit();
}

GameObject * Scene::CreateObject()
{
	auto object = new GameObject(context);
	object->Initialize(object->AddComponent<Transform>());
	AddObject(object);

	return object;
}

void Scene::AddObject(GameObject * object)
{
	objects.emplace_back(object);
	bAdded = true;
}

void Scene::RemoveObject(GameObject * object)
{
}

const bool Scene::IsExistObject(GameObject * object)
{
	return false;
}

void Scene::Update()
{
	bool bStarted = Engine::IsOnEngineFlags(EngineFlags_Game) && bEditorMode;
	bool bStopped = !Engine::IsOnEngineFlags(EngineFlags_Game) && !bEditorMode;
	bEditorMode = !Engine::IsOnEngineFlags(EngineFlags_Game);

	if (bAdded) {
		renderer->SetRenderables(this);
		bAdded = false;
	}
	if (bStarted) for (auto object : objects) object->Start();
	if (bStopped) for (auto object : objects) object->Stop();
	
	for (auto object : objects) object->Update();
}

