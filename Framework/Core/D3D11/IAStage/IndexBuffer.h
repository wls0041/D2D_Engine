#pragma once
#include "../IPipeline.h" //기본생성자는 알아서 생성되므로 cpp에서 초기화 필요 없음

class IndexBuffer : public IPipeline
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