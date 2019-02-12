#pragma once

class BaseShader
{
public:
	BaseShader(class Context* context);
	virtual ~BaseShader() {}

	virtual void BindPipeline() = 0;

protected:
	const bool CheckShaderError(const HRESULT& hr, ID3D10Blob* error);

protected:
	class Graphics* graphics;
	std::string shaderFilePath;
	std::string functionName;
};