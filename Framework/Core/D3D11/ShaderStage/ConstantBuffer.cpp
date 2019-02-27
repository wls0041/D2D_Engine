#include "Framework.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Context * context)
	: cbuffer(nullptr)
{
	graphics = context->GetSubsystem<Graphics>();
}

ConstantBuffer::~ConstantBuffer()
{
	Clear();
}

void ConstantBuffer::Clear()
{
	SAFE_RELEASE(cbuffer);
}

void ConstantBuffer::Unmap()
{
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);
}

void ConstantBuffer::BindPipeline(const ShaderType & type, const uint & slot)
{
	auto dc = graphics->GetDeviceContext();

	switch (type)
	{
	case ShaderType::VS: dc->VSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::PS: dc->PSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::GS: dc->GSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::CS: dc->CSSetConstantBuffers(slot, 1, &cbuffer); break;
	}
}
