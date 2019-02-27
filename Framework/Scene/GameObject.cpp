#include "Framework.h"
#include "GameObject.h"
#include "./Component/Transform.h"

GameObject::GameObject(Context * context) : context(context), transform(nullptr), name(""), bActive(true)
{
}

GameObject::~GameObject()
{
	for (auto component : components) {
		component->OnDestroy();
		SAFE_DELETE(component);
	}
	components.clear();
	components.shrink_to_fit();
}

void GameObject::Initialize(Transform * transform)
{
	this->transform = transform;
}

void GameObject::Start()
{
	if (bActive) {
		for (auto component : components) component->OnStart();
	}
}

void GameObject::Update()
{
	if (bActive) {
		for (auto component : components) component->OnUpdate();
	}
}

void GameObject::Stop()
{
	if (bActive) {
		for (auto component : components) component->OnStop();
	}
}

const bool GameObject::HasComponent(const ComponentType & type)
{
	for (auto component : components) {
		if (component->GetComponentType() == type) return true;
	}
	return false;
}
