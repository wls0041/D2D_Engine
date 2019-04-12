#pragma once
#include "IWidget.h"

class Widget_ProgressBar final : public IWidget
{
public:
	Widget_ProgressBar(class Context *context);
	~Widget_ProgressBar() = default;

	Widget_ProgressBar(const Widget_ProgressBar&) = delete;
	Widget_ProgressBar &operator=(const Widget_ProgressBar&) = delete;

	void Begin() override;
	void Render() override;

private:
	std::string status;
	float progress;
};