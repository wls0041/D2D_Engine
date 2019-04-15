#pragma once

class FileSystem
{
public:
	static const char *EveryFilter; //dialog를 열었을 때 원하는 유형만 나오도록 필터링
	static const char *TextureFilter;
	static const char *ShaderFilter;
	static const char *XmlFilter;

public:
	static void OpenFileDialog(std::function<void(std::string)> func, const char *filter = EveryFilter, const char *directory = "");
	static void SaveFileDialog(std::function<void(std::string)> func, const char *filter = EveryFilter, const char *directory = "");

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
	static const std::string GetPathWithoutExtension(const std::string& path);
	static const std::string GetRelativeFilePath(const std::string &absolutePath);
	static const std::string GetParentDirectory(const std::string &directory);
	static const std::string GetWorkingDirectory();

	static const std::vector<std::string> &GetSupportImageFormats() { return supportImageFormats; }
	static const std::vector<std::string> &GetSupportAudioFormats() { return supportAudioFormats; }
	static const std::vector<std::string> &GetSupportBinaryFormats() { return supportBinaryFormats; }

	static const std::vector<std::string> GetDirectoriesInDirectory(const std::string &directory); //directory반환
	static const std::vector<std::string> GetFilesInDirectory(const std::string &directory); //directory 안의 file 반환

	static const bool IsSupportedImageFile(const std::string &path);
	static const bool IsSupportedAudioFile(const std::string &path);
	static const bool IsSupportedBinaryFile(const std::string &path);

	static const std::string ToUpper(const std::string& lower);
	static const std::string ToLower(const std::string& upper);
	static const std::string ToString(const std::wstring& wstr);
	static const std::wstring ToWString(const std::string& str);

	static void Replace_All(std::string &inStr, const std::string &from, const std::string &to);
private:
	static std::vector<std::string> supportImageFormats;
	static std::vector<std::string> supportAudioFormats;
	static std::vector<std::string> supportBinaryFormats;
};