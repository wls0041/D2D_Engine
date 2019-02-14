#pragma once
#include "IComponent.h"

class Collider final : public IComponent
{
public:
	Collider(class Context* context, class GameObject *object, class Transform *transform);
	virtual ~Collider();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

	class Transform* GetTransform() const { return transform; }
	const Vector3& GetCenter() const { return center; }
	const Vector3& GetSize() const { return size; }
	const BoundBox GetBoundBox() const;

	void SetTransform(class Transform* transform) { this->transform = transform; }
	void SetCenter(const Vector3& center) { this->center = center; }
	void SetSize(const Vector3& size) { this->size = size; }

	const bool AABB(Collider* collider);

public:
	std::function<void()> Event;

private:
	class Context* context;
	class Transform* transform;
	Vector3 center;
	Vector3 size;
};