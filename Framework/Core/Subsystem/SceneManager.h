#pragma once
#include "ISubsystem.h"

class SceneManager : public ISubsystem
{
public:
	SceneManager(class Context* context);
	virtual ~SceneManager();

	const bool Initialize() override;

	void Update();

	class Scene* GetCurrentScene() const { return currentScene; }
	void SetCurrentScene(const std::string& name);

	void RegisterScene(const std::string& name, class Scene* scene);

private:
	class Scene* currentScene;
	std::map<std::string, class Scene*> scenes;
};