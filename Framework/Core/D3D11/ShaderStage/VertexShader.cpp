#include "Framework.h"
#include "VertexShader.h"
#include "../DX11_Helper.h"

VertexShader::VertexShader(Context * context) : shader(nullptr), blob(nullptr), path(""), entryPoint(""), shaderModel("")
{
	graphics = context->GetSubsystem<Graphics>();
}

VertexShader::~VertexShader()
{
	Clear();
}

void VertexShader::Create(const std::string & path, const std::string & entryPoint, const std::string & shaderModel, D3D_SHADER_MACRO * macros)
{
	this->path = path;
	this->entryPoint = entryPoint;
	this->shaderModel = shaderModel;

	DX11_Helper::CompileShader(path, entryPoint, shaderModel, macros, &blob);
	auto hr = graphics->GetDevice()->CreateVertexShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));
}

void VertexShader::Clear()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(blob);
}

void VertexShader::BindPipeline()
{
	graphics->GetDeviceContext()->VSSetShader(shader, nullptr, 0);
}
