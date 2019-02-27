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
