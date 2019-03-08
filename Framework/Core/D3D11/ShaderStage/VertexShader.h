#pragma once
#include "BaseShader.h"
#include "../IPipeline.h"

class VertexShader : public BaseShader, public IPipeline
{
public:
	VertexShader(class Context* context);
	virtual ~VertexShader();

	ID3D10Blob* GetBlob() const { return blob; }

	void Create
	(
		const std::string& filePath,
		const std::string& functionName = "VS"
	);
	void Clear();

	void BindPipeline() override;

private:
	ID3D11VertexShader* shader;
	ID3D10Blob* blob;
};