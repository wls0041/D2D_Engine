#pragma once
#include "IWidget.h"

class Widget_Hierarchy final : public IWidget
{
public:
	Widget_Hierarchy(class Context* context);
	~Widget_Hierarchy();

	void Render() override;

private:

};