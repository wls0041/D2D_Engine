#pragma once

//GUID(Globally Unique Identifier) MS만 사용하는 식별자(전역 고유 식별자)
//UUID(Universally Unique Identifier) 여러 운영체제에 사용가능한 식별자(범용 고유 식별자)

class GUIDGenerator final
{
public:
	static const uint Generate();
	static const std::string GenerateToSring();
};