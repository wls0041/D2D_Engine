#include "Framework.h"
#include "Pipeline.h"

Pipeline::Pipeline(Context * context)
	: context(context),
	  vertexBuffer(nullptr),
	  indexBuffer(nullptr),
	  inputLayout(nullptr),
	  primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	  vertexShader(nullptr),
	  pixelShader(nullptr),
	  bVertexBuffer(false),
	  bIndexBuffer(false),
	  bInputLayout(false),
	  bPrimitiveTopology(false),
	  bVertexShader(false),
	  bPixelShader(false)
{
	graphics = context->GetSubsystem<Graphics>();
}

Pipeline::~Pipeline()
{
}

void Pipeline::SetVertexBuffer(VertexBuffer * vertexBuffer)
{
	if (this->vertexBuffer != vertexBuffer) {
		this->vertexBuffer = vertexBuffer;
		this->bVertexBuffer = true;
	}
}

void Pipeline::SetIndexBuffer(IndexBuffer * indexBuffer)
{
	if (this->indexBuffer != indexBuffer) {
		this->indexBuffer = indexBuffer;
		this->bIndexBuffer = true;
	}
}

void Pipeline::SetInputLayout(InputLayout * inputLayout)
{
	if (this->inputLayout != inputLayout) {
		this->inputLayout = inputLayout;
		this->bInputLayout = true;
	}
}

void Pipeline::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY & primitiveTopology)
{
	if (this->primitiveTopology != primitiveTopology) {
		this->primitiveTopology = primitiveTopology;
		this->bPrimitiveTopology = true;
	}
}

void Pipeline::SetVertexShader(VertexShader * vertexShader)
{
	if (this->vertexShader != vertexShader) {
		this->vertexShader = vertexShader;
		this->bVertexShader = true;
	}
}

void Pipeline::SetPixelShader(PixelShader * pixelShader)
{
	if (this->pixelShader != pixelShader) {
		this->pixelShader = pixelShader;
		this->bPixelShader = true;
	}
}

void Pipeline::SetVSConstantBuffer(ConstantBuffer * constantBuffer)
{
	vs_constantBuffers.emplace_back(constantBuffer);
}

void Pipeline::SetPSConstantBuffer(ConstantBuffer * constantBuffer)
{
	ps_constantBuffers.emplace_back(constantBuffer);
}

void Pipeline::BindPipeline()
{
	IAStage();
	VSStage();
	RSStage();
	PSStage();
	OMStage();
}

void Pipeline::Draw(const uint & vertexCount, const uint & startVertexLocation)
{
	graphics->GetDeviceContext()->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(const uint & indexCount, const uint & startIndexLocation, const uint & startVertexLocation)
{
	graphics->GetDeviceContext()->DrawIndexed(indexCount, startIndexLocation, startVertexLocation);
}
