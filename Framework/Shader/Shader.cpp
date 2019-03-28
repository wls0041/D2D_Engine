#include "Framework.h"
#include "Shader.h"

Shader::Shader(Context * context) : context(context), inputLayout(nullptr)
{
}

Shader::~Shader()
{
	SAFE_DELETE(inputLayout);

	for (auto &shader : shaders) SAFE_DELETE(shader.second);
	
	defines.clear();
	shaders.clear();
}

auto Shader::GetVertexShader() -> VertexShader *
{
	if (!shaders.count[ShaderType::VS]) return nullptr;

	return reinterpret_cast<VertexShader*>(shaders[ShaderType::VS]);
}

auto Shader::GetPixelShader() -> PixelShader *
{
	if (!shaders.count[ShaderType::PS]) return nullptr;

	return reinterpret_cast<PixelShader*>(shaders[ShaderType::PS]);
}

void Shader::AddShader(const ShaderType & type, const std::string & path, const std::string & entryPoint, const std::string & shaderModel)
{
	std::vector<D3D_SHADER_MACRO> macros;
	for (const auto &define : defines) macros.emplace_back(D3D_SHADER_MACRO{ define.first.c_str(), define.second.c_str() });

	macros.emplace_back(D3D_SHADER_MACRO{ nullptr, nullptr }); //항상 마지막에는 null이 들어가야 함. 그렇게 만들어져 있음

	switch (type)
	{
	case ShaderType::VS:
	{
		//_Pairib --> pair<iterator, bool>
		auto pair = shaders.insert(std::make_pair(type, new VertexShader(context)));
		if (pair.second) { //성공하면 true 실패하면 false가 나옴
			auto shader = reinterpret_cast<VertexShader*>(pair.first->second);
			shader->Create(path, entryPoint.length() ? entryPoint : "VS", shaderModel.length() ? shaderModel : "vs_5_0", macros.data());

			if (!inputLayout) {
				inputLayout = new InputLayout(context);
				inputLayout->Create(shader->GetBlob());
			}
			else {
				inputLayout->Clear();
				inputLayout->Create(shader->GetBlob());
			}
		}
	}
		break;
	case ShaderType::PS:
	{
		auto pair = shaders.insert(std::make_pair(type, new PixelShader(context)));
		if (pair.second) { //성공하면 true 실패하면 false가 나옴
			auto shader = reinterpret_cast<PixelShader*>(pair.first->second);
			shader->Create(path, entryPoint.length() ? entryPoint : "PS", shaderModel.length() ? shaderModel : "ps_5_0", macros.data());
		}
	}
		break;
	case ShaderType::GS:
		break;
	case ShaderType::CS:
		break;
	default:
		break;
	}
}

void Shader::AddDefine(const std::string & define, const std::string & value)
{
	defines[define] = value;
}

void Shader::Clear()
{
	for (const auto &shader : shaders) Clear(shader.first);
}

void Shader::Clear(const ShaderType & type)
{
	switch (type)
	{
	case ShaderType::VS:
		reinterpret_cast<VertexShader*>(shaders[type])->Clear();
		inputLayout->Clear();
		break;
	case ShaderType::PS:
		reinterpret_cast<PixelShader*>(shaders[type])->Clear();
		inputLayout->Clear();
		break;
	case ShaderType::GS:
		break;
	case ShaderType::CS:
		break;
	default:
		break;
	}
}
