#pragma once
#include "Framework.h"

enum class ResourceType : uint
{
	Unknown,
	Texture,
	Shader,
	Mesh,
	Animation,
	Audio,
	Material,
};

class IResource
{
public:
	template <typename T>
	static const ResourceType DeduceResourceType();

public:
	IResource(class Context* context)
		: context(context)
		, name("")
		, filePath("")
		, resourceType(ResourceType::Unknown)
	{
		resourceMgr = context->GetSubsystem<class ResourceManager>();
	}
	virtual ~IResource() {}

	const std::string& GetResourceName() const { return name; }
	const std::string& GetResourcePath() const { return filePath; }
	const ResourceType& GetResourceType() const { return resourceType; }

	void SetResourceName(const std::string& name) { this->name = name; }
	void SetResourcePath(const std::string& filePath) { this->filePath = filePath; }
	void SetResourceType(const ResourceType& type) { this->resourceType = type; }

	virtual void SaveToFile(const std::string& filePath) = 0;
	virtual void LoadFromFile(const std::string& filePath) = 0;

protected:
	class Context* context;
	class ResourceManager* resourceMgr;

	std::string name;
	std::string filePath;
	ResourceType resourceType;
};

template<typename T>
inline const ResourceType IResource::DeduceResourceType()
{
	std::string id = typeid(T).name();

	ResourceType enumType = ResourceType::Unknown;

	if (id.find("Texture") != std::string::npos)			enumType = ResourceType::Texture;
	else if (id.find("Shader") != std::string::npos)		enumType = ResourceType::Shader;
	else if (id.find("Mesh") != std::string::npos)		enumType = ResourceType::Mesh;
	else if (id.find("Animation") != std::string::npos)	enumType = ResourceType::Animation;
	else if (id.find("Audio") != std::string::npos)		enumType = ResourceType::Audio;
	else if (id.find("Material") != std::string::npos)	enumType = ResourceType::Material;

	return enumType;
}
