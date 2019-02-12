#include "Framework.h"
#include "VertexShader.h"

VertexShader::VertexShader(Context * context)
	: BaseShader(context)
	, shader(nullptr)
	, blob(nullptr)
{
}

VertexShader::~VertexShader()
{
	Clear();
}

void VertexShader::Create(const std::string & filePath, const std::string & functionName)
{
	this->shaderFilePath = filePath;
	this->functionName = functionName;

	ID3D10Blob* error = nullptr;
	HRESULT hr = D3DX11CompileFromFileA
	(
		shaderFilePath.c_str(),
		nullptr,
		nullptr,
		functionName.c_str(),
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		nullptr,
		&blob,
		&error,
		nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreateVertexShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(error);
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
