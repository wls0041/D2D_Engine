#pragma once
#include "../IPipeline.h"

class VertexShader final : public IPipeline
{
public:
	VertexShader(class Context* context);
	~VertexShader();

	ID3D10Blob* GetBlob() const { return blob; }

	void Create
	(
		const std::string& path,
		const std::string& entryPoint = "VS",
		const std::string& shaderModel = "vs_5_0",
		D3D_SHADER_MACRO *macros = nullptr
	);
	void Clear();

	void BindPipeline();

private:
	class Graphics *graphics;
	ID3D11VertexShader* shader;
	ID3D10Blob* blob;
	std::string path;
	std::string entryPoint;
	std::string shaderModer;
};