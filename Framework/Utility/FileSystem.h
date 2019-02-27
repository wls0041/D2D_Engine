#pragma once

class FileSystem
{
public:
	static const bool Create_Directory(const std::string &path);
	static const bool Delete_Directory(const std::string &directory);
	static const bool Delete_File(const std::string &path);
	static const bool Copy_File(const std::string &src, const std::string &dst);
	static const bool IsDirectory(const std::string &path);
	static const bool Exist_Directory(const std::string &directory);
	static const bool Exist_File(const std::string &path);

	static const std::string GetFileNameFromFilePath(const std::string& path);
	static const std::string GetIntactFileNameFromFilePath(const std::string& path);
	static const std::string GetDirectoryFromFilePath(const std::string& path);
	static const std::string GetExtensionFromFilePath(const std::string& path);
	static const std::string GetRelativeFilePath(const std::string &absolutePath);
	static const std::string GetParentDirectory(const std::string &directory);
	static const std::string GetWorkingDirectory();

	static const std::string ToUpper(const std::string& lower);
	static const std::string ToLower(const std::string& upper);
	static const std::string ToString(const std::wstring& wstr);
	static const std::wstring ToWString(const std::string& str);

private:
	static std::vector<std::string> supportImageFormats;
	static std::vector<std::string> supportAudioFormages;
	static std::vector<std::string> supportBinaryFormats;
};