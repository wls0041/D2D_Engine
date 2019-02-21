#pragma once

class Scene
{
public:
	Scene(class Context* context);
	virtual ~Scene();

	const std::string& GetName() const { return name; }
	class GameObject *GetSceneCamera() const { return sceneCamera; }
	const std::vector<class GameObject*> &GetObjects() const { return objects; }

	void SetName(const std::string& name) { this->name = name; }

	class GameObject *CreateObject();
	void AddObject(class GameObject *object);
	void RemoveObject(class GameObject *object);
	const bool IsExistObject(class GameObject *object);

	void Update();

private:
	class Context* context;
	class Renderer *renderer;
	class GameObject *sceneCamera;
	std::string name;
	bool bEditorMode;
	bool bAdded;

	std::vector<class GameObject*> objects;
};