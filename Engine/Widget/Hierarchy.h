#pragma once
#include "IWidget.h"

class Hierarchy final : public IWidget
{
public:
	Hierarchy(class Context* context);
	~Hierarchy();

	void Render() override;

private:

};