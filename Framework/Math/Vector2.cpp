#include "Framework.h"
#include "Vector2.h"
#include "Vector3.h"

const Vector2 Vector2::Zero(0.0f);
const Vector2 Vector2::One(1.0f);

Vector2::Vector2(const Vector3 & rhs) : x(rhs.x), y(rhs.y)
{
}

void Vector2::Normalize()
{
	float factor = Length();
	factor = 1.0f / factor;

	*this *= factor;
}
