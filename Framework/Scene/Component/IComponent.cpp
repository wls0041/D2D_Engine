#include "Framework.h"
#include "IComponent.h"
#include "Animator.h"
#include "AudioSource.h"
#include "Camera.h"
#include "Collider.h"
#include "Renderable.h"
#include "Script.h"

#define REGISER_COMPONENT_TYPE(T, enumT) template<> inline const ComponentType IComponent::DeduceComponentType<T>() { return enumT; }

REGISER_COMPONENT_TYPE(Animator, ComponentType::Animator)
REGISER_COMPONENT_TYPE(AudioSource, ComponentType::AudioSource)
REGISER_COMPONENT_TYPE(Camera, ComponentType::Camera)
REGISER_COMPONENT_TYPE(Collider, ComponentType::Collider)
REGISER_COMPONENT_TYPE(Renderable, ComponentType::Renderable)
REGISER_COMPONENT_TYPE(Script, ComponentType::Script)
REGISER_COMPONENT_TYPE(Transform, ComponentType::Transform)