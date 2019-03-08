#pragma once
#include "BaseShader.h"
#include "../IPipeline.h"

class PixelShader : public BaseShader, public IPipeline
{
public:
	PixelShader(class Context* context);
	virtual ~PixelShader();

	void Create
	(
		const std::string& filePath,
		const std::string& functionName = "PS"
	);
	void Clear();

	void BindPipeline() override;

private:
	ID3D11PixelShader* shader;
};