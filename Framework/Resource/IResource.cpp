#include "Framework.h"
#include "IResource.h"

IResource::IResource(Context * context)
	: context(context)
	, resourceName("")
	, resourcePath("")
	, resourceType(ResourceType::Unknown)
{
	resourceMgr = context->GetSubsystem<ResourceManager>();
}

template <typename T>
constexpr ResourceType IResource::DeduceResourceType()
{
	return ResourceType::Unknown;
}

//명시적 템플릿 인스턴스(템플릿 특수화)
#define REGISTER_RESOURCE_TYPE(T, enumT) template <> ResourceType IResource::DeduceResourceType<T>() { return enumT; }

REGISTER_RESOURCE_TYPE(Texture, ResourceType::Texture)
REGISTER_RESOURCE_TYPE(Animation, ResourceType::Animation)
REGISTER_RESOURCE_TYPE(AudioClip, ResourceType::Audio)
REGISTER_RESOURCE_TYPE(Material, ResourceType::Material)
REGISTER_RESOURCE_TYPE(Mesh, ResourceType::Mesh)
//REGISTER_RESOURCE_TYPE(Shader, ResourceType::Shader)