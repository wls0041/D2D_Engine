#pragma once
#include "IWidget.h"

class Widget_Menubar final : public IWidget
{
public:
	Widget_Menubar(class Context* context);
	~Widget_Menubar();

	void Begin() override {};
	void End() override {};
	void Render() override;

private:
	bool bShowMetricWindow;
	bool bShowStyleEditor;
	bool bShowDemoWindow;
};