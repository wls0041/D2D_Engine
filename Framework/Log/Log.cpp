#include "Framework.h"
#include "Log.h"
#include "ILogger.h"

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
	va_end(args); //args의 공간을 null로 초기화. 혹시 모를 에러 방지

	Write(buffer, LogType::Info);
}

void Log::WriteFormatWarning(const char * text, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer), text, args);
	va_end(args); //args의 공간을 null로 초기화. 혹시 모를 에러 방지

	Write(buffer, LogType::Warning);
}

void Log::WriteFormatError(const char * text, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer), text, args);
	va_end(args); //args의 공간을 null로 초기화. 혹시 모를 에러 방지

	Write(buffer, LogType::Error);
}


void Log::Write(const char * text, const LogType & type)
{
	logger ? LogString(text, type) : LogToFile(text, type);
}

void Log::Write(const std::string & text, const LogType & type)
{
	Write(text.c_str(), type);
}

void Log::Write(const bool & value, const LogType & type)
{
	value ? Write("True", type) : Write("False", type);
}

void Log::Write(const int & value, const LogType & type)
{
	Write(std::to_string(value), type);
}

void Log::Write(const uint & value, const LogType & type)
{
	Write(std::to_string(value), type);
}

void Log::Write(const float & value, const LogType & type)
{
	Write(std::to_string(value), type);
}

void Log::Write(const double & value, const LogType & type)
{
	Write(std::to_string(value), type);
}

void Log::LogString(const char * text, const LogType & type)
{
	std::lock_guard<std::mutex> guard(logMutex);//생성될 때 락, 소멸될 때 언락(지역이므로 이 함수가 끝날 때 같이 소멸함)
	logger->Log(std::string(text), static_cast<int>(type));
}

void Log::LogToFile(const char * text, const LogType & type)
{
	std::lock_guard<std::mutex> guard(logMutex);
	std::string prefix = type == LogType::Info ? "Info    : " : type == LogType::Warning ? "Warning : " : "Error   : "; //접두사(Info: )
	std::string finalText = prefix + text;

	out.open("Log.txt", std::ios::out | std::ios::app);
	{
		out << finalText << std::endl;
	}
	out.close();
}