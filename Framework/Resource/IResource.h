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