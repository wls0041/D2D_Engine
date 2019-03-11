#pragma once
#include "Framework.h"

namespace DX11_Helper //class�� �ƴ� namespace�� ��. cpp������ ���. ��������� �ߺ���
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

	//entrypoint -> ������(�Լ��̸�), shader_macro -> shader���� ����� ��ũ��(������)���� ����, 
	inline const bool CompileShader(const std::string &path, const std::string &entryPoint, const std::string &shaderModel, D3D_SHADER_MACRO *macros, ID3DBlob **blob) { 
#ifndef OPTIMIZATION
		int shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#else 
		int shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3; //�˾Ƽ� ����ȭ���Ѷ� (level3 -> �ִ���. ���. ���� �ӵ��� ����)
#endif
		ID3DBlob *error = nullptr;
		auto hr = D3DCompileFromFile
		(
			FileSystem::ToWString(path).c_str(),
			macros,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,  //file_include -> �̸� �������� ����� ���Խ�ų �� �ְ� �ϴ� �÷���
			entryPoint.c_str(),
			shaderModel.c_str(), //target. ��� shader���� �޾ƿ� ���ΰ�
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
		return  //��Ʈ�������� �ϳ��� ��ħ
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