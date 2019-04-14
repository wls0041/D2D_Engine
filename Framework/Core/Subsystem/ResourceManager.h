#pragma once
#include "ISubsystem.h"
#include "../../Resource/IResource.h"

class ResourceManager : public ISubsystem
{
public:
	ResourceManager(class Context* context);
	virtual ~ResourceManager();

	const bool Initialize() override;

	auto GetImageimporter() const -> class ImageImporter* { return imageImporter; }

	template <typename T> T* Load(const std::string& filePath);
	template <typename T> T* GetResourceFromName(const std::string& name);
	template <typename T> T* GetResourceFromPath(const std::string& path);
	template <typename T> std::vector<T*> GetResourcesFromType();
	std::vector<IResource*> GetAllResource();

	void RegisterResource(IResource* resource);
	void RegisterDirectory(const ResourceType& type, const std::string& directory);

private:
	class ImageImporter *imageImporter;

	typedef std::vector<IResource*> ResourceGroup;
	std::map<ResourceType, ResourceGroup> resourceGroups;
	std::map<ResourceType, std::string> resourceDirectories;
};

template<typename T>
inline T * ResourceManager::Load(const std::string & filePath)
{
	auto resource = GetResourceFromPath<T>(filePath);

	if (!resource)
	{
		auto type = IResource::DeduceResourceType<T>();
		auto directory = resourceDirectories[type];
		
		resource = new T(context);
		resource->SetResourceType(type);
		resource->SetResourcePath(filePath);
		resource->SetResourceName(FileSystem::GetIntactFileNameFromFilePath(filePath));
		resource->LoadFromFile(directory + filePath);

		RegisterResource(resource);
	}

	return resource;
}

template<typename T>
inline T * ResourceManager::GetResourceFromName(const std::string & name)
{
	for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourceName() == name)
			return static_cast<T*>(resource);
	}
	return nullptr;
}

template<typename T>
inline T * ResourceManager::GetResourceFromPath(const std::string & path)
{
	for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourcePath() == path)
			return static_cast<T*>(resource);
	}
	return nullptr;
}

template<typename T>
inline std::vector<T*> ResourceManager::GetResourcesFromType()
{
	ResourceType type = IResource::DeduceResourceType<T>();

	std::vector<T*> tempResources;
	for (auto resource : resourceGroups[type])
	{
		if (resource->GetResourceType() != type)
			continue;

		tempResources.emplace_back(static_cast<T*>(resource));
	}

	return tempResources;
}
