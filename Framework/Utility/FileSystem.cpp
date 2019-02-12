#include "Framework.h"
#include "FileSystem.h"

//find() - �־��� ���ڿ��� �����ϴ� ��ġ -> ������
//rfind() - �־��� ���ڿ��� �����ϴ� ��ġ -> ������
//find_first_of() - �־��� �����߿� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_of() - �־��� �����߿� �ϳ��� �ɸ��� ������ ��ġ
//find_first_not_of() - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_not_of() - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ������ ��ġ

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
