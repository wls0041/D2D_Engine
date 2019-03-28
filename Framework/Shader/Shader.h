#pragma once

class Shader final
{
public:
	Shader(class Context *context);
	~Shader();

	Shader(const Shader&) = delete;
	Shader &operator=(const Shader&) = delete;

	auto GetVertexShader() -> VertexShader*;
	auto GetPixelShader() -> PixelShader*;
	auto GetInputLayout() const -> InputLayout* { return inputLayout; }

	void AddShader(const ShaderType &type, const std::string &path, const std::string &entryPoint = "", const std::string &shaderModel = "" );
	void AddDefine(const std::string &define, const std::string &value = "1");

	void Clear();
	void Clear(const ShaderType &type);

private:
	class Context *context;
	InputLayout *inputLayout;
	
	std::map<ShaderType, void*> shaders;
	std::map<std::string, std::string> defines;
};