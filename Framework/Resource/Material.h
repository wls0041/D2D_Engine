#pragma once
#include "IResource.h"

class Material : public IResource
{
public:
	Material(class Context* context);
	virtual ~Material();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	class VertexShader* GetVertexShader() const { return vertexShader; }
	class PixelShader* GetPixelShader() const { return pixelShader; }
	class Texture* GetDiffuseTexture() const { return diffuseTexture; }
	const Color& GetDiffuseColor() const { return diffuseColor; }

	void SetShader(const std::string& filePath);
	void SetVertexShader(class VertexShader* vertexShader) { this->vertexShader = vertexShader; }
	void SetPixelShader(class PixelShader* pixelShader) { this->pixelShader = pixelShader; }
	void SetDiffuseTexture(class Texture* diffuseTexture) { this->diffuseTexture = diffuseTexture; }
	void SetDiffuseColor(const Color& diffuseColor) { this->diffuseColor = diffuseColor; }

private:
	class VertexShader* vertexShader;
	class PixelShader* pixelShader;
	class Texture* diffuseTexture;
	Color diffuseColor;
};