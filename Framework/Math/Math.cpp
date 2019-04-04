#include "Framework.h"
#include "Math.h"
#include <random>

const float Math::PI		= 3.14159265359f;
const float Math::PI_2		= 6.28318530718f;
const float Math::PI_DIV_2	= 1.57079632679f;
const float Math::PI_DIV_4	= 0.78539816339f;
const float Math::TO_RAD	= PI / 180.0f;
const float Math::TO_DEG	= 180.0f / PI;

std::random_device rd;
std::mt19937 randomEngine(rd());

const float Math::Random(const float & min, const float & max)
{
	//Uniform Initialization - 중괄호 초기화. c++11부터 추가, 생성과 동시에 초기화 시킴, 변수이름 필요없이 바로 초기화 가능.
	return std::uniform_real_distribution<float> {min, max}(randomEngine);  //정규분포

	//정석
	//std::uniform_real_distribution<float> test(min, max);
	//float random = test(randomEngine);
}

const int Math::Random(const int & min, const int & max)
{
	return std::uniform_int_distribution<int> {min, max}(randomEngine);
}

//ceil()
//floor()
//round()
