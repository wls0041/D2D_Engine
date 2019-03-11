#pragma once
#include "Framework.h"

namespace DX11_Helper //class가 아닌 namespace일 뿐. cpp에서만 사용. 헤더에서는 중복남
{
	inline const bool CheckShaderError(const HRESULT & hr, ID3D10Blob * error)
	{
		if (FAILED(hr))
		{
			if (error != nullptr)
			{
				std::string str = reinterpret_cast<const char*>(error->GetBufferPointer());
				Log::WriteFormatError("Shader : %s", str.c_str());
			}
			return false;
		}

		return true;
	}

	//entrypoint -> 진입점(함수이름), shader_macro -> shader에서 사용할 매크로(디파인)값을 넣음, 
	inline const bool CompileShader(const std::string &path, const std::string &entryPoint, const std::string &shaderModel, D3D_SHADER_MACRO *macros, ID3DBlob **blob) { 
#ifndef OPTIMIZATION
		int shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#else 
		int shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3; //알아서 최적화시켜라 (level3 -> 최대한. 대신. 빌드 속도가 느림)
#endif
		ID3DBlob *error = nullptr;
		auto hr = D3DCompileFromFile
		(
			FileSystem::ToWString(path).c_str(),
			macros,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,  //file_include -> 미리 컴파일한 헤더를 포함시킬 수 있게 하는 플래그
			entryPoint.c_str(),
			shaderModel.c_str(), //target. 어느 shader에서 받아온 모델인가
			shaderFlags,
			0,
			blob,
			&error
		);
		assert(SUCCEEDED(hr));
		SAFE_RELEASE(error);

		return CheckShaderError(hr, error);
	}

	inline const uint CreateBitMask(const bool &blendEnable, const Factor &srcBlend, const Factor &destBlend, const Operation &blendOp,
									const Factor &srcBlendAlpha, const Factor &destBlendAlpha, const Operation &blendOpAlpha, const ColorMask &renderTargetWriteMask = ColorMask::ALL) {
		return  //비트연산으로 하나로 합침
			static_cast<uint>(blendEnable) |
			static_cast<uint>(srcBlend) |
			static_cast<uint>(destBlend) |
			static_cast<uint>(blendOp) |
			static_cast<uint>(srcBlendAlpha) |
			static_cast<uint>(destBlendAlpha) |
			static_cast<uint>(blendOpAlpha) |
			static_cast<uint>(renderTargetWriteMask);
	}

}