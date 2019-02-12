#include "Framework.h"
#include "PixelShader.h"

PixelShader::PixelShader(Context * context)
	: BaseShader(context)
	, shader(nullptr)
{
}

PixelShader::~PixelShader()
{
	Clear();
}

void PixelShader::Create(const std::string & filePath, const std::string & functionName)
{
	this->shaderFilePath = filePath;
	this->functionName = functionName;

	ID3D10Blob* error = nullptr, *blob = nullptr;
	HRESULT hr = D3DX11CompileFromFileA
	(
		shaderFilePath.c_str(),
		nullptr,
		nullptr,
		functionName.c_str(),
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		nullptr,
		&blob,
		&error,
		nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreatePixelShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(error);
	SAFE_RELEASE(blob);
}

void PixelShader::Clear()
{
	SAFE_RELEASE(shader);
}

void PixelShader::BindPipeline()
{
	graphics->GetDeviceContext()->PSSetShader(shader, nullptr, 0);
}
