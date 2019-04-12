#include "stdafx.h"
#include "Widget_ProgressBar.h"
#include "./Resource/ProgressReport.h"

Widget_ProgressBar::Widget_ProgressBar(Context * context) : IWidget(context), status(""), progress(0.0f)
{
	title = "Hold On...";
	xMin = 500.0f;
	yMin = 83.0f;
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking;
}

void Widget_ProgressBar::Begin()
{
	auto &report = ProgressReport::Get();
	auto bLoadingScene = report.IsLoading(ProgressReport::Scene);

	if (bLoadingScene) {
		progress = report.GetPercentage(ProgressReport::Scene);
		status = report.GetStatus(ProgressReport::Scene);
	}

	bVisible = bLoadingScene;
	
	IWidget::Begin();
}

void Widget_ProgressBar::Render()
{
	if (!bVisible) return;

	ImGui::SetWindowFocus();
	ImGui::PushItemWidth(xMin - ImGui::GetStyle().WindowPadding.x * 2.0f);
	ImGui::ProgressBar(progress, ImVec2(0, 0));
	ImGui::TextUnformatted(status.c_str());
	ImGui::PopItemWidth();
}
