#pragma once
#include "Framework.h"

enum class ComponentType : uint {
	Unknown,
	Camera,
	Transform,
	Animator,
	AudioSource,
	Collider,
	Renderable,
	Script,
};

class IComponent
{
public:
	template <typename T>
	static const ComponentType DeduceComponentType();
public:
	IComponent(class Context *context, class GameObject *object, class Transform *transform) //object -> ��� �ҼӵǾ����� �˱� ���� ����, transform->��ü�� ��ġ ����
		: context(context), object(object), transform(transform), componentType(ComponentType::Unknown), bEnabled(true) {}
	virtual ~IComponent() {}

	virtual void OnInitialize() = 0;
	virtual void OnStart() = 0; //�����ϴ� ����
	virtual void OnUpdate() = 0;
	virtual void OnStop() = 0; //���� ��
	virtual void OnDestroy() = 0; 

	class GameObject *GetGameObject() const { return object; }
	class Transform *GetTrnasform() const { return transform; }
	const ComponentType &GetComponentType() const { return componentType; }
	const bool &IsEnabled() const { return bEnabled; }

	void SetComponenetType(const ComponentType &type) { this->componentType = type; }
	void SetIsEnabled(const bool &bEnabled) { this->bEnabled = bEnabled; }

protected:
	class Context *context;
	class GameObject *object;
	class Transform *transform;
	ComponentType componentType;
	bool bEnabled;
};

template<typename T>
inline const ComponentType IComponent::DeduceComponentType()
{
	auto type = typeid(T).name();
	auto enumType = ComponentType::Unknown;

	if (type.find("Camera") != std::npos) enumType = ComponentType::Camera;
	else if (type.find("Transform") != std::npos) enumType = ComponentType::Transform;
	else if (type.find("Animator") != std::npos) enumType = ComponentType::Animator;
	else if (type.find("AudioSource") != std::npos) enumType = ComponentType::AudioSource;
	else if (type.find("Collider") != std::npos) enumType = ComponentType::Collider;
	else if (type.find("Renderable") != std::npos) enumType = ComponentType::Renderable;
	else if (type.find("Script") != std::npos) enumType = ComponentType::Script;

	return enumType;
}