#pragma once
#include "IComponent.h"

class Light final : public IComponent
{
public:
	Light(class Context *context, class GameObject *object, class Transform *transform);
	~Light();

	Light(const Light&) = delete;
	Light &operator=(const Light&) = delete;

private:
	Color color;
	float minDistance;
	float maxDistance;
	float distanceFactor; //�Ÿ� �� ���� �������� ȿ���� �ֱ� ���� factor
	bool bTwinkle;
};