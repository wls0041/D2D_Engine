#include "Framework.h"
#include "Color.h"

Color::Color(const uint & rhs)
{
	const float f = 1.0f / 255.0f;
	//1. and연산으로 추출
	r = f * ((rhs >> 16) & 0x000000ff);
	g = f * ((rhs >> 8) & 0x000000ff);
	b = f * ((rhs >> 0) & 0x000000ff);
	a = f * ((rhs >> 24) & 0x000000ff);

	//2. 1byte로 캐스팅해서 상위 정보를 날린 후 다시 캐스팅
	//r = f * static_cast<float>(static_cast<unsigned char<((rhs >> 16)));
	//g = f * static_cast<float>(static_cast<unsigned char<((rhs >> 8)));
	//b = f * static_cast<float>(static_cast<unsigned char<((rhs >> 0)));
	//a = f * static_cast<float>(static_cast<unsigned char<((rhs >> 24)));
}

Color::operator uint() const
{
	uint tempR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : static_cast<uint>(r * 255.0f);
	uint tempG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : static_cast<uint>(g * 255.0f);;
	uint tempB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : static_cast<uint>(b * 255.0f);;
	uint tempA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : static_cast<uint>(a * 255.0f);;

	return (tempA << 24) | (tempR << 16) | (tempG << 8) | (tempB << 0);
}