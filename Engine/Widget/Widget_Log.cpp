#include "stdafx.h"
#include "Widget_Log.h"

Widget_Log::Widget_Log(Context * context) : IWidget(context), maxLogCount(100), bShowInfo(true), bShowWarning(true), bShowError(true), bScroll(false)
{
	title = "Log";
	
	logger = new EngineLogger();
	logger->SetCallback([this](const LogInfo &logInfo) { AddLog(logInfo); });

	Log::SetLogger(logger);
}

Widget_Log::~Widget_Log()
{
	SAFE_DELETE(logger);
}

void Widget_Log::Render()
{
	if (!bVisible) return;

	static ImGuiTextFilter logFilter;
	static const std::vector<ImVec4> logColor =
	{
		ImVec4(0.76f, 0.77f, 0.8f, 1.0f), //Info
		ImVec4(0.75f, 0.75f, 0.0f, 1.0f), //Warning
		ImVec4(0.75f, 0.0f, 0.0f, 1.0f), //Error
	};

	auto LogButton = [this](const IconType& type, bool* toggle)
	{
		ImGui::SameLine();
		ImGui::PushStyleColor
		(
			ImGuiCol_Button,
			ImGui::GetStyle().Colors[*toggle ? ImGuiCol_ButtonActive : ImGuiCol_Button]
		);

		if (IconProvider::Get().ImageButton(type, 15.0f))
		{
			*toggle = !(*toggle);
			bScroll = true;
		}

		ImGui::PopStyleColor();
	};

	if (ImGui::Button("Clear"))
		Clear();

	LogButton(IconType::Log_Info, &bShowInfo);
	LogButton(IconType::Log_Warning, &bShowWarning);
	LogButton(IconType::Log_Error, &bShowError);

	ImGui::SameLine();
	logFilter.Draw("Filter", -100.0f);
	ImGui::Separator();

	ImGui::BeginChild("LogBoard", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		for (auto& log : logs)
		{
			if (!logFilter.PassFilter(log.text.c_str()))
				continue;

			bool bCheck = false;
			bCheck |= log.type == 0 && bShowInfo;
			bCheck |= log.type == 1 && bShowWarning;
			bCheck |= log.type == 2 && bShowError;

			if (bCheck)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, logColor[log.type]);
				ImGui::TextUnformatted(log.text.c_str());
				ImGui::PopStyleColor();
			}
		}

		if (bScroll)
		{
			ImGui::SetScrollHereY();
			bScroll = false;
		}
	}
	ImGui::EndChild();
}

void Widget_Log::AddLog(const LogInfo & logInfo)
{
	logs.push_back(logInfo);
	if (logs.size() > maxLogCount) logs.pop_front();
	bScroll = true;
}

void Widget_Log::Clear()
{
	logs.clear();
	logs.shrink_to_fit();
}
