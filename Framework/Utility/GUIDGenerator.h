#pragma once

//GUID(Globally Unique Identifier) MS�� ����ϴ� �ĺ���(���� ���� �ĺ���)
//UUID(Universally Unique Identifier) ���� �ü���� ��밡���� �ĺ���(���� ���� �ĺ���)

class GUIDGenerator final
{
public:
	static const uint Generate();
	static const std::string GenerateToSring();
};