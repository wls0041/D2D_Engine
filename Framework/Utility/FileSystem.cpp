#include "Framework.h"
#include "FileSystem.h"
#include <filesystem> //window17 �߰�

using namespace std::filesystem;

//find() - �־��� ���ڿ��� �����ϴ� ��ġ -> ������
//rfind() - �־��� ���ڿ��� �����ϴ� ��ġ -> ������
//find_first_of() - �־��� �����߿� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_of() - �־��� �����߿� �ϳ��� �ɸ��� ������ ��ġ
//find_first_not_of() - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_not_of() - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ������ ��ġ

const char *FileSystem::EveryFilter = "Every File(*.*)\0*.*;\0"; //*(�𸣴� ���� �̸�) . *(�𸣴� Ȯ����)
const char *FileSystem::TextureFilter = "Texture File(*.png, *.jpg, *bmp)\0*.png;*.jpg;*.bmp;\0";
const char *FileSystem::ShaderFilter = "Shader File(*.hlsl)\0*.hlsl;\0";
const char *FileSystem::XmlFilter = "Xml File(*.xml)\0*.xml;\0";


std::vector<std::string> FileSystem::supportImageFormats = { ".png", ".jpg", ".bmp" };
std::vector<std::string> FileSystem::supportAudioFormats = { ".aiff", ".asf", ".asx", ".dls", ".flac", ".fsb", ".it", ".m3u", ".midi", ".mod", 
															 ".mp2", ".mp3", ".ogg", ".pls", ".s3m", ".vag",  ".wav", ".wax", ".wma", ".xm", ".xma"};
std::vector<std::string> FileSystem::supportBinaryFormats = {};

const std::string FileSystem::GetFileNameFromFilePath(const std::string & path)
{
	//C:\\SGA\\2D\\Tree.png  -------> Tree.png
	auto lastIndex = path.find_last_of("\\/");
	auto fileName = path.substr(lastIndex + 1, path.length());

	return fileName;
}

const std::string FileSystem::GetIntactFileNameFromFilePath(const std::string & path)
{
	//Tree.png ----> Tree
	auto fileName = GetFileNameFromFilePath(path);
	auto lastIndex = fileName.find_last_of('.');
	auto intactFileName = fileName.substr(0, lastIndex);

	return intactFileName;
}

const std::string FileSystem::GetDirectoryFromFilePath(const std::string & path)
{
	//C:\\SGA\\2D\\Tree.png  -------> C:\\SGA\\2D\\;
	auto lastIndex = path.find_last_of("\\/");
	auto directory = path.substr(0, lastIndex + 1);

	return directory;
}

const std::string FileSystem::GetExtensionFromFilePath(const std::string & path)
{
	//C:\\SGA\\2D\\Tree.png --------> .png
	auto lastIndex = path.find_last_of('.');

	if (lastIndex != std::string::npos)
		return path.substr(lastIndex, path.length());

	return "";
}

const std::string FileSystem::GetRelativeFilePath(const std::string & absolutePath)
{
	//���� ��θ� ����
	path p = absolute(absolutePath);
	path r = absolute(GetWorkingDirectory());

	//��Ʈ ��ΰ� �ٸ���� ������ ��ȯ
	if (p.root_path() != r.root_path())
		return p.generic_string();

	path result;

	//�� ��ΰ� �������� ������ üũ
	path::const_iterator iter_path = p.begin();
	path::const_iterator iter_relative = r.begin();

	while (
		*iter_path == *iter_relative &&
		iter_path != p.end() &&
		iter_relative != r.end())
	{
		iter_path++;
		iter_relative++;
	}

	//relative�� ���� �� ��ū������ ..�� �߰�
	if (iter_relative != r.end())
	{
		iter_relative++;
		while (iter_relative != r.end())
		{
			result /= "..";
			iter_relative++;
		}
	}

	//���� ��� �߰�
	while (iter_path != p.end())
	{
		result /= *iter_path;
		iter_path++;
	}

	return result.generic_string();
}

const std::string FileSystem::GetParentDirectory(const std::string & directory)
{
	auto lastIndex = directory.find_last_of("\\/");
	auto result = directory;

	if (lastIndex == std::string::npos)
		return result;

	if (lastIndex == directory.length() - 1)
	{
		result = result.substr(0, lastIndex - 1);
		return GetParentDirectory(result);
	}

	return result.substr(0, lastIndex) + "/";
}

const std::string FileSystem::GetWorkingDirectory()
{
	return current_path().generic_string() + "/";
}

const std::vector<std::string> FileSystem::GetDirectoriesInDirectory(const std::string & directory)
{
	std::vector<std::string> subDirectories;

	directory_iterator iter(directory);
	for (auto p : iter ) {
		if (!is_directory(p.status())) continue; //������ �ƴϸ� continue
		subDirectories.emplace_back(p.path().generic_string());
	}

	return subDirectories;
}

const std::vector<std::string> FileSystem::GetFilesInDirectory(const std::string & directory)
{
	std::vector<std::string> files;

	directory_iterator iter(directory);
	for (auto p : iter ) {
		if (!is_regular_file(p.status())) continue; //����ȭ�� ���� �ƴϸ�(ex.���� ����) continue
		files.emplace_back(p.path().generic_string());
	}

	return files;
}

const bool FileSystem::IsSupportedImageFile(const std::string & path)
{
	std::string fileExtenstion = GetExtensionFromFilePath(path);

	auto supportFormats = GetSupportImageFormats();
	for (const auto &format : supportFormats) {
		if (format == fileExtenstion || ToUpper(format) == fileExtenstion) return true;
	}
	return false;
}

const bool FileSystem::IsSupportedAudioFile(const std::string & path)
{
	std::string fileExtenstion = GetExtensionFromFilePath(path);

	auto supportFormats = GetSupportAudioFormats();
	for (const auto &format : supportFormats) {
		if (format == fileExtenstion || ToUpper(format) == fileExtenstion) return true;
	}
	return false;
}

const bool FileSystem::IsSupportedBinaryFile(const std::string & path)
{
	std::string fileExtenstion = GetExtensionFromFilePath(path);

	auto supportFormats = GetSupportBinaryFormats();
	for (const auto &format : supportFormats) {
		if (format == fileExtenstion || ToUpper(format) == fileExtenstion) return true;
	}
	return false;
}

const std::string FileSystem::ToUpper(const std::string & lower)
{
	std::string upper = "";
	for (const auto& character : lower)
		upper += toupper(character);

	return upper;
}

const std::string FileSystem::ToLower(const std::string & upper)
{
	std::string lower = "";
	for (const auto& character : upper)
		lower += tolower(character);

	return lower;
}

const std::string FileSystem::ToString(const std::wstring & wstr)
{
	std::string result = "";
	result.assign(wstr.begin(), wstr.end());
	return result;
}

const std::wstring FileSystem::ToWString(const std::string & str)
{
	std::wstring result = L"";
	result.assign(str.begin(), str.end());
	return result;
}

void FileSystem::Replace_All(std::string & inStr, const std::string & from, const std::string & to)
{
	size_t startPos = 0;
	while ((startPos = inStr.find(from, startPos)) != std::string::npos) { //script�� ���� Ư�� �ܾ ã�� ���ϴ� �ܾ�� ��ȯ
		inStr.replace(startPos, from.length(), to);
		startPos += to.length(); //��ȯ�� �ܾ� ���ڼ���ŭ Ŀ���� �Űܼ� �ߺ� ����
	}
}

void FileSystem::OpenFileDialog(std::function<void(std::string)> func, const char * filter, const char * directory)
{
	char buffer[255];
	ZeroMemory(buffer, 255);

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = Settings::Get().GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = buffer; //������ ������ ��ΰ� ����� ����
	ofn.nMaxFile = 255; //�ּҰ��� 255��.
	ofn.lpstrInitialDir = directory; //dialog�� �� ó������ ������ ���丮
	ofn.Flags = OFN_NOCHANGEDIR; //���������� �۾��� ���丮 ����

	//TRUE -> define�� 4byte¥��. bool(true) -> 1byte
	if (GetOpenFileNameA(&ofn) == TRUE) {
		std::string fileName = GetRelativeFilePath(buffer);//GetFileNameFromFilePath -> ������, GetRelativeFilePath -> �����
		if (func != nullptr) func(fileName);
	}
}

void FileSystem::SaveFileDialog(std::function<void(std::string)> func, const char * filter, const char * directory)
{
	char buffer[255];
	ZeroMemory(buffer, 255);

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = Settings::Get().GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = buffer; //������ ������ ��ΰ� ����� ����
	ofn.nMaxFile = 255; //�ּҰ��� 255��.
	ofn.lpstrInitialDir = directory; //dialog�� �� ó������ ������ ���丮
	ofn.Flags = OFN_NOCHANGEDIR; //���������� �۾��� ���丮 ����

	//TRUE -> define�� 4byte¥��. bool(true) -> 1byte
	if (GetSaveFileNameA(&ofn) == TRUE) {
		std::string fileName = GetRelativeFilePath(buffer);
		if (func != nullptr) func(fileName);
	}
}

const bool FileSystem::Create_Directory(const std::string & path)
{
	bool bResult = false;

	try{//�õ��ؼ� �����ϸ� �Ѿ
		bResult = create_directories(path); //������ ����
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Delete_Directory(const std::string & directory)
{
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = remove_all(directory) > 0; //���� ������ ���빰�� �����ϱ� ����, �ڽ��� ���� ������ ���� ��ȯ
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), directory.c_str());
	}
	return bResult;
}

const bool FileSystem::Delete_File(const std::string & path)
{
	if (is_directory(path)) return false; //���°� ���丮�� �������� ������ �̸� ó��
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = remove(path); //���� ����
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Copy_File(const std::string & src, const std::string & dst)
{
	if (src == dst) return false; //�̹� ����Ǿ� �ִٸ� ����
	if (!Exist_Directory(GetDirectoryFromFilePath(dst))) Create_Directory(GetDirectoryFromFilePath(dst));
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = copy_file(src, dst, copy_options::overwrite_existing); //path�� ������ġ�ΰ� ���丮��ġ�ΰ� �Ǵ�
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : Copy_File - %s", error.what());
	}
	return bResult;
}

const bool FileSystem::IsDirectory(const std::string & path)
{
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = is_directory(path); //path�� ������ġ�ΰ� ���丮��ġ�ΰ� �Ǵ�
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : IsDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Exist_Directory(const std::string & directory)
{
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = exists(directory); 
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : Exist_Directory - %s, %s", error.what(), directory.c_str());
	}
	return bResult;
}

const bool FileSystem::Exist_File(const std::string & path)
{
	bool bResult = false;

	try {//�õ��ؼ� �����ϸ� �Ѿ
		bResult = exists(path);
	}
	catch (filesystem_error error) {//�����ϸ� ���⿡ ����
		Log::WriteFormatError("FileSystem : Exist_File - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}
