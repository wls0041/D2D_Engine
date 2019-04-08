#include "Framework.h"
#include "Pipeline.h"

Pipeline::Pipeline(Context * context)
	: context(context)
	, vertexBuffer(nullptr)
	, indexBuffer(nullptr)
	, inputLayout(nullptr)
	, primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, vertexShader(nullptr)
	, pixelShader(nullptr)
	, blendState(nullptr)
	, bVertexBuffer(false)
	, bIndexBuffer(false)
	, bInputLayout(false)
	, bPrimitiveTopology(false)
	, bVertexShader(false)
	, bPixelShader(false)
	, bBlendState(false)
{
	graphics = context->GetSubsystem<Graphics>();
	CreateBlendStates();
}

Pipeline::~Pipeline()
{
	for (auto &state : blendStates) SAFE_DELETE(state.second);
}

void Pipeline::SetVertexBuffer(VertexBuffer * buffer)
{
    if (!buffer)
    {
        Log::Error("VertexBuffer : Invalid parameter");
        return;
    }

    if (vertexBuffer)
    {
        if (vertexBuffer->GetID() == buffer->GetID())
            return;
    }

    vertexBuffer = buffer;
    bVertexBuffer = true;
}

void Pipeline::SetIndexBuffer(IndexBuffer * buffer)
{
    if (!buffer)
    {
        Log::Error("IndexBuffer : Invalid parameter");
        return;
    }

    if (indexBuffer)
    {
        if (indexBuffer->GetID() == buffer->GetID())
            return;
    }

    indexBuffer = buffer;
    bIndexBuffer = true;
}

void Pipeline::SetInputLayout(InputLayout * layout)
{
    if (!layout)
    {
        Log::Error("InputLayout : Invalid parameter");
        return;
    }

    if (inputLayout)
    {
        if (inputLayout->GetID() == layout->GetID())
            return;
    }

    inputLayout = layout;
    bInputLayout = true;
}

void Pipeline::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY & topology)
{
    if (primitiveTopology == topology)
        return;

    primitiveTopology = topology;
    bPrimitiveTopology = true;
}

void Pipeline::SetVertexShader(VertexShader * shader)
{
    if (!shader)
    {
        Log::Error("VertexShader : Invalid parameter");
        return;
    }

    if (vertexShader)
    {
        if (vertexShader->GetID() == shader->GetID())
            return;
    }

    vertexShader = shader;
    bVertexShader = true;
}

void Pipeline::SetVSConstantBuffer(ConstantBuffer * buffer)
{
    if (!buffer)
    {
        Log::Error("VSConstantBuffer : Invalid parameter");
        return;
    }

    vs_constantBuffers.emplace_back(buffer->GetBuffer());
}

void Pipeline::SetVSShaderResource(Texture * texture)
{
    if (!texture)
    {
        Log::Error("VSShaderResource : Invalid parameter");
        return;
    }

    vs_shaderResources.emplace_back(texture->GetShaderResourceView());
}

void Pipeline::SetVSShaderResource(ID3D11ShaderResourceView * srv)
{
    vs_shaderResources.emplace_back(srv);
}

void Pipeline::SetPixelShader(PixelShader * shader)
{
    if (!shader)
    {
        Log::Error("PixelShader : Invalid parameter");
        return;
    }

    if (pixelShader)
    {
        if (pixelShader->GetID() == shader->GetID())
            return;
    }

    pixelShader = shader;
    bPixelShader = true;
}

void Pipeline::SetPSConstantBuffer(ConstantBuffer * buffer)
{
    if (!buffer)
    {
        Log::Error("PSConstantBuffer : Invalid parameter");
        return;
    }

    ps_constantBuffers.emplace_back(buffer->GetBuffer());
}

void Pipeline::SetPSShaderResource(Texture * texture)
{
    if (!texture)
    {
        Log::Error("PSShaderResource : Invalid parameter");
        return;
    }

    ps_shaderResources.emplace_back(texture->GetShaderResourceView());
}

void Pipeline::SetPSShaderResource(ID3D11ShaderResourceView * srv)
{
    ps_shaderResources.emplace_back(srv);
}

void Pipeline::SetBlendState(const BlendMode & blendMode)
{
	if (blendStates.find(blendMode) == blendStates.end()) {
		Log::Error("Pipeline::SetBlendState - Invaild Parameter");
		return;
	}
	auto state = blendStates[blendMode];

	if (blendState) {
		if (blendState->GetID() == state->GetID()) return;
	}

	blendState = state;
	bBlendState = true;
}

void Pipeline::BindPipeline()
{
    IAStage();
    VSStage();
    RSStage();
    PSStage();
    OMStage();
}

void Pipeline::Draw()
{
    graphics->GetDeviceContext()->Draw
    (
        vertexBuffer->GetVertexCount(),
        0
    );
}

void Pipeline::Draw(const uint & vertexCount, const uint & vertexOffset)
{
	graphics->GetDeviceContext()->Draw
    (
        vertexCount, 
        vertexOffset
    );
}

void Pipeline::DrawIndexed()
{
    graphics->GetDeviceContext()->DrawIndexed
    (
        indexBuffer->GetIndexCount(),
        0,
        0
    );
}

void Pipeline::DrawIndexed(const uint & indexCount, const uint & indexOffset, const uint & vertexOffset)
{
	graphics->GetDeviceContext()->DrawIndexed
	(
		indexCount,
        indexOffset,
        vertexOffset
	);
}

