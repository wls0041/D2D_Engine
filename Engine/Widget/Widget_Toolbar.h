#pragma once
#include "IWidget.h"

class Widget_Toolbar final : public IWidget
{
public:
	Widget_Toolbar(class Context *context);
	~Widget_Toolbar() = default;

	Widget_Toolbar(const Widget_Toolbar&) = delete;
	Widget_Toolbar &operator=(const Widget_Toolbar&) = delete;

	void Begin() override;
	void Render() override;

private:
	void ShowRendererOptions();

private:
	class Renderer* renderer;
	float rendererOptionAlpha;
	bool bShowRendrerOption;
};
