#pragma once
#include "../IPipeline.h"

class PixelShader final : public IPipeline
{
public:
	PixelShader(class Context* context);
	~PixelShader();

	void Create
	(
		const std::string& path,
		const std::string& entryPoint = "PS",
		const std::string& shaderModel = "ps_5_0",
		D3D_SHADER_MACRO *macros = nullptr
	);
	void Clear();

	void BindPipeline();

private:
	class Graphics *graphics;
	ID3D11PixelShader* shader;
	std::string path;
	std::string entryPoint;
	std::string shaderModel;
};