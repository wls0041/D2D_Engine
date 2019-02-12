#pragma once

class VertexBuffer
{
public:
	VertexBuffer(class Context* context);
	virtual ~VertexBuffer();

	template <typename T>
	void Create
	(
		const std::vector<T>& vertices, 
		const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT	
	);
	void Clear();

	void BindPipeline();

private:
	class Graphics* graphics;

	ID3D11Buffer* vbuffer;
	uint stride;
	uint offset;
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE & usage)
{
	assert(!vertices.empty());

	Clear();

	stride = sizeof(T);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	if (usage == D3D11_USAGE_DYNAMIC)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

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
