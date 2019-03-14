#pragma once
#include "Framework.h"
#include "ILogger.h"

struct LogInfo { std::string text; int type; };

class EngineLogger final : public ILogger
{
public:
	typedef std::function<void(const LogInfo&)> LogFunc;

public:
	EngineLogger() = default;
	~EngineLogger() = default;

	EngineLogger(const EngineLogger&) = delete;
	EngineLogger &operator=(const EngineLogger&) = delete;

	void Log(const std::string &text, const int &type) override {
		LogInfo logInfo;
		logInfo.text = text;
		logInfo.type = type;

		func(logInfo);
	}
	void SetCallback(LogFunc&& func) {
		this->func = std::forward<LogFunc>(func);
	}

private:
	LogFunc func;
};