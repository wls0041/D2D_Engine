#include "Framework.h"
#include "Pipeline.h"

Pipeline::Pipeline(Context * context)
	: vertexBuffer(nullptr),
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
	//IAStage
	if (this->bVertexBuffer) {
		this->vertexBuffer->BindPipeline();
		this->bVertexBuffer = false;
	}
	if (this->bIndexBuffer) {
		this->indexBuffer->BindPipeline();
		this->bIndexBuffer = false;
	}
	if (this->bInputLayout) {
		this->inputLayout->BindPipeline();
		this->bInputLayout = false;
	}
	if (this->bPrimitiveTopology) {
		//TODO : 
		this->bPrimitiveTopology;
		this->bPrimitiveTopology = false;
	}

	//VSStage
	if (this->bVertexShader) {
		this->vertexShader->BindPipeline();
		this->bVertexShader = false;
	}
	for (uint i = 0; i < vs_constantBuffers.size(); i++) vs_constantBuffers[i]->BindPipeline(ShaderType::VS, i);
	vs_constantBuffers.clear(); //세팅 완료되면 삭제
	vs_constantBuffers.shrink_to_fit();

	//PSStage
	if (this->bPixelShader) {
		this->pixelShader->BindPipeline();
		this->bPixelShader = false;
	}
	for (uint i = 0; i < ps_constantBuffers.size(); i++) ps_constantBuffers[i]->BindPipeline(ShaderType::PS, i);
	ps_constantBuffers.clear();
	ps_constantBuffers.shrink_to_fit();
}

void Pipeline::Draw(const RenderType & renderType)
{
}
