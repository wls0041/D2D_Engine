#include "Framework.h"
#include "FileStream.h"

FileStream::FileStream(const std::string& path, const StreamMode& mode)
{
	streamMode = mode;

	switch (streamMode)
	{
	case StreamMode::Write:
		out.open(path, std::ios::out | std::ios::binary);
		if (out.fail()) {
			Log::WriteFormatError("FileStream::Open - Failed to open \"%s\" for writing", path.c_str());
		}
		break;
	case StreamMode::Read:
		in.open(path, std::ios::in | std::ios::binary);
		if (in.fail()) {
			Log::WriteFormatError("FileStream::Read - Failed to read \"%s\" for writing", path.c_str());
		}
		break;
	}
}

FileStream::~FileStream()
{
	switch (streamMode)
	{
	case StreamMode::Write:
		out.flush(); //쓰는데 사용한 데이터를 날려버림
		out.close();
		break;

	case StreamMode::Read:
		in.clear();
		in.close();
		break;
	}
}

void FileStream::Write(const std::string & value)
{
	auto length = static_cast<uint>(value.length());
	Write(length);

	out.write(value.c_str(), length);
}

void FileStream::Write(const std::vector<VertexTexture>& value)
{
	auto length = static_cast<uint>(value.size());
	Write(length);

	out.write(reinterpret_cast<const char*>(value.data()), sizeof(VertexTexture) * length);
}

void FileStream::Write(const std::vector<uint>& value)
{
	auto length = static_cast<uint>(value.size());
	Write(length);

	out.write(reinterpret_cast<const char*>(value.data()), sizeof(uint) * length);
}

void FileStream::Write(const std::vector<std::byte>& value)
{
	auto length = static_cast<uint>(value.size());
	Write(length);

	out.write(reinterpret_cast<const char*>(value.data()), sizeof(std::byte) * length);
}

void FileStream::Read(std::string & value)
{
	//uint length;  -- 정석방법
	//Read(length);

	auto length = Read<uint>(); //두 번째 template함수로 만든 방법

	value.clear();
	value.shrink_to_fit();
	value.resize(length);

	in.read(value.data(), length);
}

void FileStream::Read(std::vector<VertexTexture>& value)
{
	auto length = Read<uint>();

	value.clear();
	value.shrink_to_fit();
	value.reserve(length);
	value.resize(length);

	in.read(reinterpret_cast<char*>(value.data()), sizeof(VertexTexture) * length);
}

void FileStream::Read(std::vector<uint>& value)
{
	auto length = Read<uint>();

	value.clear();
	value.shrink_to_fit();
	value.reserve(length);
	value.resize(length);

	in.read(reinterpret_cast<char*>(value.data()), sizeof(uint) * length);
}

void FileStream::Read(std::vector<std::byte>& value)
{
	auto length = Read<uint>();

	value.clear();
	value.shrink_to_fit();
	value.reserve(length);
	value.resize(length);

	in.read(reinterpret_cast<char*>(value.data()), sizeof(std::byte) * length);
}
