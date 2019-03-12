#pragma once
#include "../IPipeline.h"

class ConstantBuffer : public IPipeline
{
public:
	ConstantBuffer(class Context* context);
	virtual ~ConstantBuffer();

	ID3D11Buffer *GetBuffer() const { return cbuffer; }

	template <typename T>
	void Create(const D3D11_USAGE& usage = D3D11_USAGE_DYNAMIC);
	void Clear();

	template <typename T>
	T* Map();
	void Unmap();

	void BindPipeline(const ShaderType& type, const uint& slot);

private:
	class Graphics* graphics;
	ID3D11Buffer* cbuffer;
};

template<typename T>
inline void ConstantBuffer::Create(const D3D11_USAGE & usage)
{
	Clear();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	if (usage == D3D11_USAGE_DYNAMIC)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(T);

	HRESULT hr = graphics->GetDevice()->CreateBuffer
	(
		&desc,
		nullptr,
		&cbuffer
	);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline T * ConstantBuffer::Map()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = graphics->GetDeviceContext()->Map
	(
		cbuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	assert(SUCCEEDED(hr));

	return reinterpret_cast<T*>(mappedResource.pData);
}
