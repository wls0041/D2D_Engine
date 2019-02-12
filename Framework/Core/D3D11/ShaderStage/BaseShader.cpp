#include "Framework.h"
#include "BaseShader.h"

BaseShader::BaseShader(Context * context)
	: shaderFilePath("")
	, functionName("")
{
	graphics = context->GetSubsystem<Graphics>();
}

const bool BaseShader::CheckShaderError(const HRESULT & hr, ID3D10Blob * error)
{
	if (FAILED(hr))
	{
		if (error != nullptr)
		{
			std::string str = reinterpret_cast<const char*>(error->GetBufferPointer());
			MessageBoxA
			(
				nullptr,
				str.c_str(),
				"SHADER ERROR!!!!",
				MB_OK
			);
		}

		return false;
	}

	return true;
}