#pragma once
#include "../IPipeline.h"

class InputLayout : public IPipeline
{
public:
	InputLayout(class Context* context);
	virtual ~InputLayout();

	void Create
	(
		D3D11_INPUT_ELEMENT_DESC* descs,
		const uint& count,
		ID3D10Blob* blob
	);
	void Create(ID3D10Blob* blob);
	void Clear();

	void BindPipeline();

private:
	class Graphics* graphics;
	ID3D11InputLayout* layout;
};