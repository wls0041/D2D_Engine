#include "Framework.h"
#include "PixelShader.h"
#include "../DX11_Helper.h"

PixelShader::PixelShader(Context * context) : shader(nullptr), path(""), entryPoint(""), shaderModer("")
{
	graphics = context->GetSubsystem<Graphics>();
}

PixelShader::~PixelShader()
{
	Clear();
}

void PixelShader::Create(const std::string & path, const std::string & entryPoint, const std::string & shaderModel, D3D_SHADER_MACRO * macros)
{
	this->path = path;
	this->entryPoint = entryPoint;
	this->shaderModer = shaderModel;

	ID3DBlob *blob = nullptr;
	DX11_Helper::CompileShader(path, entryPoint, shaderModel, macros, &blob);
	auto hr = graphics->GetDevice()->CreatePixelShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));
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
