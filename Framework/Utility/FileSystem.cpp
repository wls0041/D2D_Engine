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
