#pragma once
#include "IWidget.h"
#include "./Log/EngineLogger.h"

class Widget_Log final : public IWidget
{
public:
	Widget_Log(class Context *context);
	~Widget_Log();

	void Render() override;

	void AddLog(const LogInfo &logInfo);
	void Clear();

private:
	EngineLogger *logger;
	std::deque<LogInfo> logs;
	uint maxLogCount;
	bool bShowInfo;
	bool bShowWarning;
	bool bShowError;
	bool bScroll;
};