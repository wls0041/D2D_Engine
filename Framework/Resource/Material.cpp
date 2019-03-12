#include "Framework.h"
#include "Material.h"

Material::Material(Context * context) : IResource(context), vertexShader(nullptr), pixelShader(nullptr), inputLayout(nullptr), diffuseTexture(nullptr), diffuseColor(0.0f)
{
	vertexShader = new VertexShader(context);
	pixelShader = new PixelShader(context);
	inputLayout = new InputLayout(context);
}

Material::Material(const Material & rhs) : IResource(rhs.context), vertexShader(rhs.vertexShader), pixelShader(rhs.pixelShader), inputLayout(rhs.inputLayout), diffuseTexture(rhs.diffuseTexture), diffuseColor(rhs.diffuseColor)
{
}

Material::~Material()
{
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
}

void Material::SaveToFile(const std::string & filePath)
{
}

void Material::LoadFromFile(const std::string & filePath)
{
}

void Material::SetShader(const std::string & filePath)
{
	vertexShader->Clear();
	vertexShader->Create(filePath);

	pixelShader->Clear();
	pixelShader->Create(filePath);

	inputLayout->Clear();
	inputLayout->Create(vertexShader->GetBlob());
}

void Material::SetDiffuseTexture(const std::string & textureName)
{
	diffuseTexture = resourceMgr->Load<Texture>(textureName);
}
