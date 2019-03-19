#pragma once
#include "./Component/IComponent.h"

class GameObject final
{
public:
	GameObject(class Context* context);
	~GameObject();

	void Initialize(class Transform* transform);

	void Start();
	void Update();
	void Stop();

	class Transform* GetTransform() const { return transform; }
	const uint &GetID() const { return id; }
	const std::string& GetName() const { return name; }
	const bool& IsActive() const { return bActive; }

	void SetName(const std::string& name) { this->name = name; }
	void SetIsActive(const bool& bActive) { this->bActive = bActive; }

	template <typename T> T* AddComponent();
	template <typename T> T* GetComponent();
	template <typename T> std::vector<T*> GetComponents();

	const bool HasComponent(const ComponentType& type);

	template <typename T>
	const bool HasComponent()
	{
		return HasComponent(IComponent::DeduceComponentType<T>());
	}

	template <typename T>
	void RemoveComponent();

private:
	class Context* context;
	class Transform *transform; //최적화. 밑의 IComponent에서 transform을 가지고 있지만 빈번한 검색은 속도를 느리게 하므로 따로 저장
	uint id;
	std::string name;
	bool bActive;

	std::vector<class IComponent*> components;
};

template<typename T>
inline T * GameObject::AddComponent()
{
	ComponentType type = IComponent::DeduceComponentType<T>();

	if (HasComponent(type) && type != ComponentType::Script) return GetComponent<T>();
	components.emplace_back(new T(context, this, transform));
	
	auto newComponent = static_cast<T*>(components.back());
	newComponent->SetComponentType(type);
	newComponent->OnInitialize();

	return newComponent;
}

template<typename T>
inline T * GameObject::GetComponent()
{
	ComponentType type = IComponent::DeduceComponentType<T>();
	
	for (auto component : components) {
		if (component->GetComponentType() == type) return static_cast<T*>(component);
	}
	return nullptr;
}

template<typename T>
inline std::vector<T*> GameObject::GetComponents()
{
	ComponentType type = IComponent::DeduceComponentType<T>();

	std::vector<T*> tempComponents;

	for (auto component : components) {
		if (component->GetComponentType() != type) continue;
		tempComponents.emplace_back(static_cast<T*>(component));
	}
	return tempComponents;
}

template<typename T>
inline void GameObject::RemoveComponent()
{
	ComponentType type = IComponent::DeduceComponentType<T>();

	for (auto iter = components.begin(); iter != components.end();) {
		auto component = *iter; //정석 방법은 (*iter)를 componenet자리에 쓰는 것
		if (component->GetComponentType() == type) {
			component->OnDestroy();
			SAFE_DELETE(component);
			iter = components.erase(iter);
		}
		else iter++;
	}
}
