#include "Framework.h"
#include "Scene.h"
#include "GameObject.h"
#include "./Component/Transform.h"
#include "./Component/Camera.h"
#include "./Component/Script.h"
#include "./Component/Light.h"
#include "./Component/Tilemap.h"

Scene::Scene(Context * context)
	: context(context)
	, name("")
	, bEditorMode(true)
	, bAdded(false)
	, sceneCamera(nullptr)
{
	renderer = context->GetSubsystem<Renderer>();

	sceneCamera = new GameObject(context);
	sceneCamera->Initialize(sceneCamera->AddComponent<Transform>());
	sceneCamera->AddComponent<Camera>();
	sceneCamera->SetName("SceneCamera");

	//Script
	auto script = sceneCamera->AddComponent<Script>();
	script->SetScript("../../_Assets/Script/Move.as");

	AddObject(sceneCamera);

	auto lightObject = CreateObject();
	lightObject->SetName("Light");

	auto light = lightObject->AddComponent<Light>();
	light->SetColor({ 1, 1, 1, 1 });
	light->SetMinDistance(10.0f);
	light->SetMaxDistance(200.0f);
	light->SetDistanceFactor(2.0f);
	light->SetIsTwinkle(false);

	/////////////////////////////////////////////////////
	auto map = CreateObject();
	map->SetName("Tilemap");

	auto tilemap = map->AddComponent<Tilemap>();
	tilemap->AddTileSet("../../_Assets/Tileset/plain_tileset.jpg");
	tilemap->AddTileSet("../../_Assets/Texture/bird.png");
}

Scene::~Scene()
{
	for (auto object : objects)
		SAFE_DELETE(object);

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

	if (bAdded)
	{
		renderer->SetRenderables(this);
		bAdded = false;
	}

	if (bStarted)
	{
		for (auto object : objects)
			object->Start();
	}

	if (bStopped)
	{
		for (auto object : objects)
			object->Stop();
	}

	for (auto object : objects)
		object->Update();
}
