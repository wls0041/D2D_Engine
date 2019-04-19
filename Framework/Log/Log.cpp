#include "Framework.h"
#include "Log.h"
#include "ILogger.h"

std::string		 Log::caller;
ILogger*         Log::logger;
std::ofstream    Log::out;
std::string      Log::fileName;
std::mutex       Log::logMutex;
bool             Log::bFirstLog = false;

void Log::SetLogger(ILogger * iLogger)
{
	logger = iLogger;
}

void Log::WriteFormatInfo(const char * text, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer), text, args);
	va_end(args); //args�� ������ null�� �ʱ�ȭ. Ȥ�� �� ���� ����

	Write(buffer, LogType::Info);
}

void Log::WriteFormatWarning(const char * text, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer), text, args);
	va_end(args); //args�� ������ null�� �ʱ�ȭ. Ȥ�� �� ���� ����

	Write(buffer, LogType::Warning);
}

void Log::WriteFormatError(const char * text, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer), text, args);
	va_end(args); //args�� ������ null�� �ʱ�ȭ. Ȥ�� �� ���� ����

	Write(buffer, LogType::Error);
}


void Log::Write(const char * text, const LogType & type)
{
	auto formattedText = caller.empty() ? std::string(text) : caller + "::" + std::string(text);
	logger ? LogString(formattedText.c_str(), type) : LogToFile(formattedText.c_str(), type);

	caller.clear();
}

void Log::Write(const std::string & text, const LogType & type)
{
	Write(text.c_str(), type);
}

void Log::Write(const bool & value, const LogType & type)
{
	value ? Write("True", type) : Write("False", type);
}

void Log::LogString(const char * text, const LogType & type)
{
	std::lock_guard<std::mutex> guard(logMutex);//������ �� ��, �Ҹ�� �� ���(�����̹Ƿ� �� �Լ��� ���� �� ���� �Ҹ���)
	logger->Log(std::string(text), static_cast<int>(type));
}

void Log::LogToFile(const char * text, const LogType & type)
{
	std::lock_guard<std::mutex> guard(logMutex);
	std::string prefix = type == LogType::Info ? "Info    : " : type == LogType::Warning ? "Warning : " : "Error   : "; //���λ�(Info: )
	std::string finalText = prefix + text;

	out.open("Log.txt", std::ios::out | std::ios::app);
	{
		out << finalText << std::endl;
	}
	out.close();
}