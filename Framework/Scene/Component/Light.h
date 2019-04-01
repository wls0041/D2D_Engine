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
	float distanceFactor; //거리 당 빛이 옅어지는 효과를 주기 위한 factor
	bool bTwinkle;
};