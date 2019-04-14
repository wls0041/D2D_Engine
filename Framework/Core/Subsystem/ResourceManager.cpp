#include "Framework.h"
#include "ResourceManager.h"
#include "../../Resource/Importer/ImageImporter.h"

ResourceManager::ResourceManager(Context * context)
	: ISubsystem(context)
{
}

ResourceManager::~ResourceManager()
{
	//std::map<type, std::vector<>> ---> resourceGroups
	// pair<type, std::vector<>> --> resourceGroup
	for (auto resourceGroup : resourceGroups)
	{
		for (auto resource : resourceGroup.second)
			SAFE_DELETE(resource);
	}

	SAFE_DELETE(imageImporter);
}

const bool ResourceManager::Initialize()
{
	imageImporter = new ImageImporter(context);

	RegisterDirectory(ResourceType::Texture,	"../../_Assets/Texture/");
	RegisterDirectory(ResourceType::Shader,		"../../_Assets/Shader/");
	RegisterDirectory(ResourceType::Mesh,		"../../_Assets/Mesh/");
	RegisterDirectory(ResourceType::Animation,	"../../_Assets/Animation/");
	RegisterDirectory(ResourceType::Audio,		"../../_Assets/Audio/");
	RegisterDirectory(ResourceType::Material,	"../../_Assets/Material/");

	return true;
}

std::vector<IResource*> ResourceManager::GetAllResource()
{
	return std::vector<IResource*>();
}

void ResourceManager::RegisterResource(IResource * resource)
{
	assert(resource);
	resourceGroups[resource->GetResourceType()].emplace_back(resource);
}

void ResourceManager::RegisterDirectory(const ResourceType & type, const std::string & directory)
{
	resourceDirectories[type] = directory;
}
