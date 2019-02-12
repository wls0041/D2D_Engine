#pragma once

class IndexBuffer
{
public:
	IndexBuffer(class Context* context);
	virtual ~IndexBuffer();

	void Create
	(
		const std::vector<uint>& indices,
		const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT	
	);
	void Clear();

	void BindPipeline();

private:
	class Graphics* graphics;
	ID3D11Buffer* ibuffer;
};