#include "Framework.h"
#include "Pipeline.h"

void Pipeline::IAStage()
{
	if (this->bVertexBuffer)
	{
		this->vertexBuffer->BindPipeline();
		this->bVertexBuffer = false;
	}

	if (this->bIndexBuffer)
	{
		this->indexBuffer->BindPipeline();
		this->bIndexBuffer = false;
	}

	if (this->bInputLayout)
	{
		this->inputLayout->BindPipeline();
		this->bInputLayout = false;
	}

	if (this->bPrimitiveTopology)
	{
		//TODO :
		this->bPrimitiveTopology;
		this->bPrimitiveTopology = false;
	}
}

void Pipeline::VSStage()
{
	if (this->bVertexShader)
	{
		this->vertexShader->BindPipeline();
		this->bVertexShader = false;
	}

	for (uint i = 0; i < vs_constantBuffers.size(); i++)
		vs_constantBuffers[i]->BindPipeline(ShaderType::VS, i);

	vs_constantBuffers.clear(); //세팅 완료 시 삭제
	vs_constantBuffers.shrink_to_fit();
}

void Pipeline::RSStage()
{
}

void Pipeline::PSStage()
{
	if (this->bPixelShader)
	{
		this->pixelShader->BindPipeline();
		this->bPixelShader = false;
	}

	for (uint i = 0; i < ps_constantBuffers.size(); i++)
		ps_constantBuffers[i]->BindPipeline(ShaderType::PS, i);

	ps_constantBuffers.clear();
	ps_constantBuffers.shrink_to_fit();
}

void Pipeline::OMStage()
{
}

void Pipeline::CreateBlendStates()
{
	BlendState *state = nullptr;
	//Blend None
	state = new BlendState(context);
	{
		auto key = state->Create(false, Factor::ONE, Factor::ZERO, Operation::ADD, Factor::ONE, Factor::ZERO, Operation::ADD); //비트 연산으로 key값 생성
		blendStates[key] = state;
	}
	state = nullptr;
}
