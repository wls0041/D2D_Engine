#pragma once
#include "IWidget.h"

class Widget_Inspector final : public IWidget
{
public:
	Widget_Inspector(class Context* context);
	~Widget_Inspector() = default;

	void Render() override;

private:
	void ShowTransform(class Transform *transform);
	void ShowCamera(class Camera *camera);
	void ShowRenderable(class Renderable *renderable);
	void ShowScript(class Script *script);

private:
	void ShowAddComponent();
	void ShowComponentPopup();
};