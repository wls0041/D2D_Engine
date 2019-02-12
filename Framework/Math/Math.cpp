#include "Framework.h"
#include "Math.h"

const float Math::PI		= 3.14159265359f;
const float Math::PI_2		= 6.28318530718f;
const float Math::PI_DIV_2	= 1.57079632679f;
const float Math::PI_DIV_4	= 0.78539816339f;
const float Math::TO_RAD	= PI / 180.0f;
const float Math::TO_DEG	= 180.0f / PI;

const float Math::Random(const float & min, const float & max)
{
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float diff = max - min;
	float val = random * diff;

	return min + val;
}

const int Math::Random(const int & min, const int & max)
{
	return rand() % (max - min + 1) + min;
}

//ceil()
//floor()
//round()
