#include "Framework.h"
#include "IResource.h"

#define REGISTER_RESOURCE_TYPE(T, enumT) template <> inline const ResourceType IResource::DeduceResourceType<T>() { return enumT; }

REGISTER_RESOURCE_TYPE(Texture, ResourceType::Texture)
REGISTER_RESOURCE_TYPE(Animation, ResourceType::Animation)
REGISTER_RESOURCE_TYPE(Audio, ResourceType::Audio)
REGISTER_RESOURCE_TYPE(Material, ResourceType::Material)
REGISTER_RESOURCE_TYPE(Mesh, ResourceType::Mesh)
//REGISTER_RESOURCE_TYPE(Shader, ResourceType::Shader)