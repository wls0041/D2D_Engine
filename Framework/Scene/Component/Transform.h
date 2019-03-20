#pragma once
#include "IComponent.h"

class Transform final : public IComponent
{
public:
	Transform(class Context* context, class GameObject *object, class Transform *transform);
	virtual ~Transform();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

	const Vector3& GetLocalScale() const { return localScale; }
	const Vector3& GetLocalRotation() const { return localRotation; }
	const Vector3& GetLocalPosition() const { return localPosition; }

	const Vector3 GetScale() { return world.GetScale(); }
	const Vector3 GetRotation() { return world.GetRotation(); }
	const Vector3 GetPosition() { return world.GetTranslation(); }

	const Vector3 GetRight();
	const Vector3 GetUp();
	const Vector3 GetForward();

	const Matrix& GetLocalMatrix() const { return local; }
	const Matrix& GetWorldMatrix() const { return world; }
	const Matrix GetWorldRotationMatrix();

	Transform* GetParent() const { return parent; }
	Transform* GetRoot() { return HasParent() ? GetParent()->GetRoot() : this; }

	void SetLocalScale(const Vector3& localScale);
	void SetLocalRotation(const Vector3& localRotation);
	void SetLocalPosition(const Vector3& localPosition);

	void SetScale(const Vector3& scale);
	void SetRotation(const Vector3& rotation);
	void SetPosition(const Vector3& position);

	void SetParent(Transform* newParent);
	void AddChild(Transform* child);

	const bool HasParent() const { return parent ? true : false; }
	const bool HasChild() const { return !childs.empty(); }

	void UpdateTransform();

private:
	class Context* context;

	Vector3 localScale;
	Vector3 localRotation;
	Vector3 localPosition;

	Matrix local;
	Matrix world;

	Transform* parent;
	std::vector<Transform*> childs;
};