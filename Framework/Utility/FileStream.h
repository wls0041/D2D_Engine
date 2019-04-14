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
	template <typename T, typename = typename std::enable_if< //template ���. ���ϴ� type�� ����ϰ� �� �� ����.
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
		out.write(reinterpret_cast<const char*>(&value), sizeof(T)); //binary�� 1byte�� ���� �ޱ� ������ char������ ����
	}

	void Write(const std::string &value);
	void Write(const std::vector<struct VertexTexture> &value);
	void Write(const std::vector<uint> &value);
	void Write(const std::vector<std::byte> &value);

	//===========================Read============================
	template <typename T, typename = typename std::enable_if< //template ���. ���ϴ� type�� ����ϰ� �� �� ����.
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
		in.read(reinterpret_cast<char*>(&value), sizeof(T)); //�о ������ �ʿ��ϱ� ������ const�� ����
	}

	template <typename T, typename = typename std::enable_if< //template ���. ���ϴ� type�� ����ϰ� �� �� ����.
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