#pragma once
#include "IWidget.h"

//createQuad °¡Áü
class Widget_Hierarchy final : public IWidget
{
public:
	Widget_Hierarchy(class Context* context);
	~Widget_Hierarchy();

	void Render() override;

private:
	void ShowHierarchy();
	void AddScene(class Scene *scene);
	void AddObject(class GameObject *object);

private:
	void ShowPopup();
	void MenuPopup();

private:
	class GameObject *CreateEmpty();
	void CreateQuad();
	void CreateScreenQuad();

private:
	class SceneManager *sceneManager;
};