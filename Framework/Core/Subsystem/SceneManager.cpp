#include "Framework.h"
#include "SceneManager.h"
#include "../../Scene/Scene.h"

SceneManager::SceneManager(Context * context)
	: ISubsystem(context)
	, currentScene(nullptr)
{
	EventSystem::Get().Subscribe(EventType::Event_Update, EVENT_HANDLER(Update));
}

SceneManager::~SceneManager()
{
	////1
	//std::map<std::string, Scene*>::iterator iter = scenes.begin();
	//for (; iter != scenes.end(); iter++)
	//	SAFE_DELETE(iter->second);

	//2
	for (auto scene : scenes)
		SAFE_DELETE(scene.second);
}

const bool SceneManager::Initialize()
{
	RegisterScene("First", new Scene(context));
	SetCurrentScene("First");

	return true;
}

void SceneManager::Update()
{
	if (currentScene)
		currentScene->Update();
}

void SceneManager::SetCurrentScene(const std::string & name)
{
	assert(scenes.count(name) > 0);
	currentScene = scenes[name];
}

void SceneManager::RegisterScene(const std::string & name, Scene * scene)
{
	assert(scenes.count(name) < 1);

	scene->SetName(name);

	//1
	scenes[name] = scene;

	//2
	//scenes.insert(make_pair(name, scene));
}
