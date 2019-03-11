#pragma once
#include "../IPipeline.h" //�⺻�����ڴ� �˾Ƽ� �����ǹǷ� cpp���� �ʱ�ȭ �ʿ� ����

class IndexBuffer : public IPipeline
{
public:
	IndexBuffer(class Context* context);
	virtual ~IndexBuffer();

	const uint &GetIndexCount() const { return indexCount; }

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
	uint indexCount;
};