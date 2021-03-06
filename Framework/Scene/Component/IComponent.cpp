#include "Framework.h"
#include "IComponent.h"
#include "Camera.h"
#include "Transform.h"
#include "Animator.h"
#include "AudioSource.h"
#include "Collider.h"
#include "Renderable.h"
#include "Script.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "Tilemap.h"

IComponent::IComponent(Context * context, GameObject * object, Transform * transform)
	: context(context)
	, object(object)
	, transform(transform)
	, componentType(ComponentType::Unknown)
	, bEnabled(true)
{
}

template <typename T>
constexpr ComponentType IComponent::DeduceComponentType()
{
	return ComponentType::Unknown;
}

#define REGISTER_COMPONENT_TYPE(T, enumT) template<> ComponentType IComponent::DeduceComponentType<T>() { return enumT; }

REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera)
REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform)
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator)
REGISTER_COMPONENT_TYPE(AudioSource, ComponentType::AudioSource)
REGISTER_COMPONENT_TYPE(Collider, ComponentType::Collider)
REGISTER_COMPONENT_TYPE(Renderable, ComponentType::Renderable)
REGISTER_COMPONENT_TYPE(Script, ComponentType::Script)
REGISTER_COMPONENT_TYPE(Light, ComponentType::Light)
REGISTER_COMPONENT_TYPE(ParticleEmitter, ComponentType::Particle)
REGISTER_COMPONENT_TYPE(Tilemap, ComponentType::Tilemap)