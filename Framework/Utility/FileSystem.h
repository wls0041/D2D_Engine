#pragma once

class FileSystem
{
public:
	static const std::string GetFileNameFromFilePath(const std::string& path);
	static const std::string GetIntactFileNameFromFilePath(const std::string& path);
	static const std::string GetDirectoryFromFilePath(const std::string& path);
	static const std::string GetExtensionFromFilePath(const std::string& path);

	static const std::string ToUpper(const std::string& lower);
	static const std::string ToLower(const std::string& upper);
	static const std::string ToString(const std::wstring& wstr);
	static const std::wstring ToWString(const std::string& str);
};