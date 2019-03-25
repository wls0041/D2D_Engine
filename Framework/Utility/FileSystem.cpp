#include "Framework.h"
#include "FileSystem.h"
#include <filesystem> //window17 추가

using namespace std::filesystem;

//find() - 주어진 문자열이 존재하는 위치 -> 정방향
//rfind() - 주어진 문자열이 존재하는 위치 -> 역방향
//find_first_of() - 주어진 문자중에 하나라도 걸리는 첫번째 위치
//find_last_of() - 주어진 문자중에 하나라도 걸리는 마지막 위치
//find_first_not_of() - 주어진 문자들이 아닌 문자가 하나라도 걸리는 첫번째 위치
//find_last_not_of() - 주어진 문자들이 아닌 문자가 하나라도 걸리는 마지막 위치

const char *FileSystem::EveryFilter = "Every File(*.*)\0*.*;\0"; //*(모르는 파일 이름) . *(모르는 확장자)
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
	//절대 경로를 만듬
	path p = absolute(absolutePath);
	path r = absolute(GetWorkingDirectory());

	//루트 경로가 다를경우 절대경로 반환
	if (p.root_path() != r.root_path())
		return p.generic_string();

	path result;

	//두 경로가 갈라지는 지점을 체크
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

	//relative에 남은 각 토큰에대해 ..을 추가
	if (iter_relative != r.end())
	{
		iter_relative++;
		while (iter_relative != r.end())
		{
			result /= "..";
			iter_relative++;
		}
	}

	//남은 경로 추가
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
		if (!is_directory(p.status())) continue; //폴더가 아니면 continue
		subDirectories.emplace_back(p.path().generic_string());
	}

	return subDirectories;
}

const std::vector<std::string> FileSystem::GetFilesInDirectory(const std::string & directory)
{
	std::vector<std::string> files;

	directory_iterator iter(directory);
	for (auto p : iter ) {
		if (!is_regular_file(p.status())) continue; //정규화된 파일 아니면(ex.숨긴 파일) continue
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
	while ((startPos = inStr.find(from, startPos)) != std::string::npos) { //script를 돌며 특정 단어를 찾아 원하는 단어로 변환
		inStr.replace(startPos, from.length(), to);
		startPos += to.length(); //변환한 단어 글자수만큼 커서를 옮겨서 중복 방지
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
	ofn.lpstrFile = buffer; //선택한 파일의 경로가 여기로 들어옴
	ofn.nMaxFile = 255; //최소값이 255임.
	ofn.lpstrInitialDir = directory; //dialog열 때 처음으로 보여줄 디렉토리
	ofn.Flags = OFN_NOCHANGEDIR; //마지막으로 작업한 디렉토리 유지

	//TRUE -> define된 4byte짜리. bool(true) -> 1byte
	if (GetOpenFileNameA(&ofn) == TRUE) {
		std::string fileName = GetRelativeFilePath(buffer);//GetFileNameFromFilePath -> 절대경로, GetRelativeFilePath -> 상대경로
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
	ofn.lpstrFile = buffer; //선택한 파일의 경로가 여기로 들어옴
	ofn.nMaxFile = 255; //최소값이 255임.
	ofn.lpstrInitialDir = directory; //dialog열 때 처음으로 보여줄 디렉토리
	ofn.Flags = OFN_NOCHANGEDIR; //마지막으로 작업한 디렉토리 유지

	//TRUE -> define된 4byte짜리. bool(true) -> 1byte
	if (GetSaveFileNameA(&ofn) == TRUE) {
		std::string fileName = GetRelativeFilePath(buffer);
		if (func != nullptr) func(fileName);
	}
}

const bool FileSystem::Create_Directory(const std::string & path)
{
	bool bResult = false;

	try{//시도해서 성공하면 넘어감
		bResult = create_directories(path); //폴더를 생성
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Delete_Directory(const std::string & directory)
{
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = remove_all(directory) > 0; //폴더 삭제시 내용물도 삭제하기 위함, 자신이 지운 파일의 개수 반환
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), directory.c_str());
	}
	return bResult;
}

const bool FileSystem::Delete_File(const std::string & path)
{
	if (is_directory(path)) return false; //들어온게 디렉토리면 에러나기 때문에 미리 처리
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = remove(path); //파일 삭제
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : CreateDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Copy_File(const std::string & src, const std::string & dst)
{
	if (src == dst) return false; //이미 복사되어 있다면 나감
	if (!Exist_Directory(GetDirectoryFromFilePath(dst))) Create_Directory(GetDirectoryFromFilePath(dst));
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = copy_file(src, dst, copy_options::overwrite_existing); //path가 파일위치인가 디렉토리위치인가 판단
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : Copy_File - %s", error.what());
	}
	return bResult;
}

const bool FileSystem::IsDirectory(const std::string & path)
{
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = is_directory(path); //path가 파일위치인가 디렉토리위치인가 판단
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : IsDirectory - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}

const bool FileSystem::Exist_Directory(const std::string & directory)
{
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = exists(directory); 
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : Exist_Directory - %s, %s", error.what(), directory.c_str());
	}
	return bResult;
}

const bool FileSystem::Exist_File(const std::string & path)
{
	bool bResult = false;

	try {//시도해서 성공하면 넘어감
		bResult = exists(path);
	}
	catch (filesystem_error error) {//실패하면 여기에 잡힘
		Log::WriteFormatError("FileSystem : Exist_File - %s, %s", error.what(), path.c_str());
	}
	return bResult;
}
