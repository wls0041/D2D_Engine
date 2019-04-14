#pragma once

enum class StreamMode : uint { Write, Read };

class FileStream final
{
public:
	FileStream();
	~FileStream();

	FileStream(const FileStream&) = delete;
	FileStream &operator=(const FileStream&) = delete;

	void Open(const std::string &path, const StreamMode &mode);
	void Close();

	//===========================Write===========================
	template <typename T, typename = typename std::enable_if< //template 기법. 원하는 type만 사용하게 할 수 있음.
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, unsigned char>::value ||
		std::is_same<T, uint>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Color>::value ||
		std::is_same<T, Matrix>::value
	>::type> void Write(const T &value)
	{
		out.write(reinterpret_cast<const char*>(&value), sizeof(T)); //binary로 1byte씩 끊어 받기 때문에 char형으로 받음
	}

	void Write(const std::string &value);
	void Write(const std::vector<struct VertexTexture> &value);
	void Write(const std::vector<uint> &value);
	void Write(const std::vector<std::byte> &value);

	//===========================Read============================
	template <typename T, typename = typename std::enable_if< //template 기법. 원하는 type만 사용하게 할 수 있음.
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, unsigned char>::value ||
		std::is_same<T, uint>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Color>::value ||
		std::is_same<T, Matrix>::value
	>::type> void Read(const T &value)
	{
		in.read(reinterpret_cast<char*>(&value), sizeof(T)); //읽어서 변형이 필요하기 때문에 const가 빠짐
	}

	template <typename T, typename = typename std::enable_if< //template 기법. 원하는 type만 사용하게 할 수 있음.
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, unsigned char>::value ||
		std::is_same<T, uint>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Color>::value ||
		std::is_same<T, Matrix>::value
	>::type> const T Read()
	{
		T value;
		Read(value);

		return value;
	}

	void Read(std::string &value);
	void Read(std::vector<struct VertexTexture> &value);
	void Read(std::vector<uint> &value);
	void Read(std::vector<std::byte> &value);

private:
	StreamMode streamMode;
	std::ifstream in;
	std::ofstream out;
};