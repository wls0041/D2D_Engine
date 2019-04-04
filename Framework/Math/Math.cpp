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
	//Uniform Initialization - �߰�ȣ �ʱ�ȭ. c++11���� �߰�, ������ ���ÿ� �ʱ�ȭ ��Ŵ, �����̸� �ʿ���� �ٷ� �ʱ�ȭ ����.
	return std::uniform_real_distribution<float> {min, max}(randomEngine);  //���Ժ���

	//����
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
