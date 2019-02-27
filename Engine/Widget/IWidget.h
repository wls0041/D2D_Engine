#pragma once
#include "stdafx.h"
#include "../ImGui/Source/imgui.h"

class IWidget
{
public:
	IWidget(class Context* context)
		: context(context)
		, title("")
		, xMin(0.0f)
		, xMax(FLT_MAX)
		, yMin(0.0f)
		, yMax(FLT_MAX)
		, height(0.0f)
		, windowFlags(ImGuiWindowFlags_NoCollapse)
		, bVisible(true)
	{}
	virtual ~IWidget() {}

	virtual void Begin()
	{
		ImGui::SetNextWindowSize(ImVec2(xMin, yMin), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints
		(
			ImVec2(xMin, yMin),
			ImVec2(xMax, yMax)
		);

		ImGui::Begin(title.c_str(), &bVisible, windowFlags);
	}

	virtual void Render() = 0;

	virtual void End()
	{
		height = ImGui::GetWindowHeight(); //begin이 있어야 만들어지고 가져올 수 있음
		ImGui::End();
	}

	const float &GetHeight() const { return height; }

	const bool& IsVisible() const { return bVisible; }
	void SetIsVisible(const bool& bVisible) { this->bVisible = bVisible; }

protected:
	class Context* context;
	std::string title;
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float height;
	int windowFlags;
	bool bVisible;
};