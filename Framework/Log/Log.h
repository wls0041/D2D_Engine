#pragma once

enum class LogType : uint
{
	Info,
	Warning,
	Error,
};

class Log
{
public:
	static void SetLogger(class ILogger* iLogger);

	static void Info(const char *text);
	static void Info(const std::string &text);

	static void Warning(const char *text);
	static void Warning(const std::string &text);
	
	static void Error(const char *text);
	static void Error(const std::string &text);

	static void WriteFormatInfo(const char* text, ...); //... - ��������
	static void WriteFormatWarning(const char* text, ...);
	static void WriteFormatError(const char* text, ...);

	static void Write(const char* text, const LogType& type);
	static void Write(const std::string& text, const LogType& type);
	static void Write(const bool& value, const LogType& type);
	static void Write(const int& value, const LogType& type);
	static void Write(const uint& value, const LogType& type);
	static void Write(const float& value, const LogType& type);
	static void Write(const double& value, const LogType& type);

	static void LogString(const char* text, const LogType& type);
	static void LogToFile(const char* text, const LogType& type);

private:
	static class ILogger* logger;
	static std::ofstream out;//������ ������ ��
	static std::string fileName;
	static std::mutex logMutex;
	static bool bFirstLog;
};