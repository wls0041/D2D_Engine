#include "Framework.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Context * context)
	: vbuffer(nullptr)
	, stride(0)
	, offset(0)
	, vertexCount(0)
{
	graphics = context->GetSubsystem<Graphics>();
}

VertexBuffer::~VertexBuffer()
{
	Clear();
}

void VertexBuffer::Clear()
{
	SAFE_RELEASE(vbuffer);
	stride = 0;
	offset = 0;
	vertexCount = 0;
}

void VertexBuffer::BindPipeline()
{
	graphics->GetDeviceContext()->IASetVertexBuffers
	(
		0,
		1,
		&vbuffer,
		&stride,
		&offset
	);
}

auto VertexBuffer::Map() -> void *
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = graphics->GetDeviceContext()->Map
	(
		vbuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	assert(SUCCEEDED(hr));

	return mappedResource.pData;
}

void VertexBuffer::Unmap()
{
	graphics->GetDeviceContext()->Unmap(vbuffer, 0);
}
