#pragma once
#include "IComponent.h"

class Light final : public IComponent
{
public:
	Light(class Context *context, class GameObject *object, class Transform *transform);
	~Light();

	Light(const Light&) = delete;
	Light &operator=(const Light&) = delete;

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

	auto GetColor() const -> const Color& { return color; }
	void SetColor(const Color &color) { this->color = color; }
	void SetColor(const Vector3 &color) { this->color.r = color.x; this->color.g = color.y; this->color.b = color.z; }
	void SetIntensity(const float &intensity) { this->color.a = intensity; }

	const float &GetMinDistance() { return minDistance; }
	void SetMinDistance(const float &minDistance) { this->minDistance = minDistance; }

	const float &GetMaxDistance() { return maxDistance; }
	void SetMaxDistance(const float &maxDistance) { this->maxDistance = maxDistance; }

	const float &GetDistanceFactor() { return distanceFactor; }
	void SetDistanceFactor(const float &distanceFactor) { this->distanceFactor = distanceFactor; }

	const bool &IsTwinkle() { return bTwinkle; }
	void SetIsTwinkle(const bool &bTwinkle) { this->bTwinkle = bTwinkle; }

private:
	Color color;
	float minDistance;
	float maxDistance;
	float distanceFactor; //�Ÿ� �� ���� �������� ȿ���� �ֱ� ���� factor
	bool bTwinkle;
};