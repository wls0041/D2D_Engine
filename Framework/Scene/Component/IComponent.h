#pragma once
#include "Framework.h"

enum class ComponentType : uint
{
	Unknown,
	Camera,
	Transform,
	Animator,
	AudioSource,
	Collider,
	Renderable,
	Script,
	Light,
};

class IComponent
{
public:
	template <typename T>
	static constexpr ComponentType DeduceComponentType();

public:
	IComponent //object -> ��� �ҼӵǾ����� �˱� ���� ����, transform->��ü�� ��ġ ����
	(
		class Context* context,
		class GameObject* object,
		class Transform* transform
	);
	virtual ~IComponent() = default;

	virtual void OnInitialize() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnStop() = 0;
	virtual void OnDestroy() = 0;

	class GameObject* GetGameObject() const { return object; }
	class Transform* GetTransform() const { return transform; }
	const ComponentType& GetComponentType() const { return componentType; }
	const bool& IsEnabled() const { return bEnabled; }

	void SetComponentType(const ComponentType& type) { this->componentType = type; }
	void SetIsEnabled(const bool& bEnabled) { this->bEnabled = bEnabled; }

protected:
	class Context* context;
	class GameObject* object;
	class Transform* transform;
	ComponentType componentType;
	bool bEnabled;
};