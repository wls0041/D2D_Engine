#pragma once
#include "IResource.h"

class Texture : public IResource
{
public:
	Texture(class Context* context);
	virtual ~Texture();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	const Vector2 GetSize() const { return Vector2(width, height); }

	void CreateTextureFromFile(const std::string& filePath);
	void CreateTexture2D
	(
		const float& width, 
		const float& height,
		const DXGI_FORMAT& format
	);
	void Clear();

	void BindPipeline(const ShaderType& type, const uint& slot);

private:
	class Graphics* graphics;
	ID3D11ShaderResourceView* srv;
	std::string filePath;
	DXGI_FORMAT format;
	float width;
	float height;
};