#pragma once

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
	static constexpr ResourceType DeduceResourceType();

public:
	IResource(class Context* context);
	virtual ~IResource() = default;

	const std::string& GetResourceName() const { return resourceName; }
	const std::string& GetResourcePath() const { return resourcePath; }
	const ResourceType& GetResourceType() const { return resourceType; }

	void SetResourceName(const std::string& name) { this->resourceName = name; }
	void SetResourcePath(const std::string& path) { this->resourcePath = path; }
	void SetResourceType(const ResourceType& type) { this->resourceType = type; }

	virtual void SaveToFile(const std::string& filePath) = 0;
	virtual void LoadFromFile(const std::string& filePath) = 0;

protected:
	class Context* context;
	class ResourceManager* resourceMgr;

	std::string resourceName;
	std::string resourcePath;
	ResourceType resourceType;
};