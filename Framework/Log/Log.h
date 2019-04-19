#pragma once

enum class LogType : uint
{
	Info,
	Warning,
	Error,
};

#define LOG_INFO(text)    { Log::caller = __FUNCTION__; Log::Write(text, LogType::Info); }  //__function__함수 객체~이름까지 나옴
#define LOG_ERROR(text)   { Log::caller = __FUNCTION__; Log::Write(text, LogType::Error); }
#define LOG_WARNING(text) { Log::caller = __FUNCTION__; Log::Write(text, LogType::Warning); } 

#define LOG_FINFO(text, ...)    { Log::caller = __FUNCTION__; Log::WriteFormatInfo(text, __VA_ARGS__); } //valiable arguments 가변인자 매크로 
#define LOG_FERROR(text, ...)   { Log::caller = __FUNCTION__; Log::WriteFormatError(text, __VA_ARGS__); }
#define LOG_FWARNING(text, ...) { Log::caller = __FUNCTION__; Log::WriteFormatWarning(text, __VA_ARGS__); }

class Log
{
public:
	static void SetLogger(class ILogger* iLogger);

	static void WriteFormatInfo(const char* text, ...); //... - 가변인자
	static void WriteFormatWarning(const char* text, ...);
	static void WriteFormatError(const char* text, ...);

	static void Write(const char* text, const LogType& type);
	static void Write(const std::string& text, const LogType& type);

	template <typename T, typename = typename std::enable_if<
		std::is_same<T, int>::value ||
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value
	>::type> static void Write(const T &value, const LogType &type) { 
		Write(std::to_string(value), type); 
	}

	static void Write(const bool& value, const LogType& type);

	static void LogString(const char* text, const LogType& type);
	static void LogToFile(const char* text, const LogType& type);

public:
	static std::string caller;

private:
	static class ILogger* logger;
	static std::ofstream out;//파일을 밖으로 뺌
	static std::string fileName;
	static std::mutex logMutex;
	static bool bFirstLog;
};