#pragma once
#include "../IPipeline.h"

class VertexBuffer : public IPipeline
{
public:
	VertexBuffer(class Context* context);
	virtual ~VertexBuffer();

	const uint &GetVertexCount() const { return vertexCount; }

	template <typename T>
	void Create
	(
		const std::vector<T>& vertices, 
		const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT	
	);
	void Clear();

	void BindPipeline();

	void *Map();
	void Unmap();

private:
	class Graphics* graphics;

	ID3D11Buffer* vbuffer;
	uint stride;
	uint offset;
	uint vertexCount;
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE & usage)
{
	assert(!vertices.empty());

	Clear();

	stride = sizeof(T);
	vertexCount = vertices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(T) * vertices.size();

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = vertices.data();

	HRESULT hr = graphics->GetDevice()->CreateBuffer
	(
		&desc,
		&subData,
		&vbuffer
	);
	assert(SUCCEEDED(hr));
}
