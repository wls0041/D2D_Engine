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
	class Timer *timer;
	class Renderer *renderer;
	class SceneManager *sceneMgr;

	Vector2 framePos;
	Vector2 frameSize;
	float time_scene_last_change;
};