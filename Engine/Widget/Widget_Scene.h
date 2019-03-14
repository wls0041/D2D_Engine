#pragma once
#include "IWidget.h"

class Widget_Scene final : public IWidget
{
public:
	Widget_Scene(class Context* context);
	~Widget_Scene() = default;

	void Render() override;

private:
	void ShowFrame();
	void ShowGizmo();

private:
	void Picking();
	void DragDropEvent();

private:
	class Renderer *renderer;
	class SceneManager *sceneMgr;
	class Camera *camera;

	Vector2 framePos;
	Vector2 frameSize;
};