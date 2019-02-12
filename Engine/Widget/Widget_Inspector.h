#pragma once
#include "IWidget.h"

class Widget_Inspector final : public IWidget
{
public:
	Widget_Inspector(class Context* context);
	~Widget_Inspector();

	void Render() override;

private:

};