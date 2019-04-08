#include "Framework.h"
#include "Pipeline.h"

void Pipeline::IAStage()
{
	if (bVertexBuffer)
	{
		vertexBuffer->BindPipeline();
		bVertexBuffer = false;
	}

	if (bIndexBuffer)
	{
		indexBuffer->BindPipeline();
		bIndexBuffer = false;
	}

	if (bInputLayout)
	{
		inputLayout->BindPipeline();
		bInputLayout = false;
	}

	//if (bPrimitiveTopology)
	{
		graphics->GetDeviceContext()->IASetPrimitiveTopology(primitiveTopology);
		bPrimitiveTopology = false;
	}
}

void Pipeline::VSStage()
{
	if (this->bVertexShader)
	{
		this->vertexShader->BindPipeline();
		this->bVertexShader = false;
	}

	//constantBuffer
	if (!vs_constantBuffers.empty())
	{
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, vs_constantBuffers.size(), vs_constantBuffers.data()); //3인자. const 더블버퍼 -> 포인터의 배열을 받겠다(포인터들의 포인터)

		vs_constantBuffers.clear(); //세팅 완료 시 삭제
		vs_constantBuffers.shrink_to_fit();
	}

	//shaderResource
	if (!vs_shaderResources.empty())
	{
		graphics->GetDeviceContext()->VSSetShaderResources(0, vs_shaderResources.size(), vs_shaderResources.data());

		vs_shaderResources.clear(); //세팅 완료 시 삭제
		vs_shaderResources.shrink_to_fit();
	}

	//sampler
	if (!vs_samplers.empty())
	{
		graphics->GetDeviceContext()->VSSetSamplers(0, vs_samplers.size(), vs_samplers.data());

		vs_samplers.clear(); //세팅 완료 시 삭제
		vs_samplers.shrink_to_fit();
	}
}

void Pipeline::RSStage()
{
}

void Pipeline::PSStage()
{
	if (bPixelShader)
	{
		pixelShader->BindPipeline();
		bPixelShader = false;
	}

	//constantBuffer
	if (!ps_constantBuffers.empty())
	{
		graphics->GetDeviceContext()->PSSetConstantBuffers(0, ps_constantBuffers.size(), ps_constantBuffers.data()); //3인자. const 더블버퍼 -> 포인터의 배열을 받겠다(포인터들의 포인터)

		ps_constantBuffers.clear(); //세팅 완료 시 삭제
		ps_constantBuffers.shrink_to_fit();
	}

	//shaderResource
	if (!ps_shaderResources.empty())
	{
		graphics->GetDeviceContext()->PSSetShaderResources(0, ps_shaderResources.size(), ps_shaderResources.data());

		ps_shaderResources.clear(); //세팅 완료 시 삭제
		ps_shaderResources.shrink_to_fit();
	}

	//sampler
	if (!ps_samplers.empty())
	{
		graphics->GetDeviceContext()->PSSetSamplers(0, ps_samplers.size(), ps_samplers.data());

		ps_samplers.clear(); //세팅 완료 시 삭제
		ps_samplers.shrink_to_fit();
	}
}

void Pipeline::OMStage()
{
	if (bBlendState) {
		blendState->BindPipeline();
		bBlendState = false;
	}
}

void Pipeline::CreateBlendStates()
{
	BlendState *state = nullptr;
	//Blend None
	state = new BlendState(context);
	{
		auto key = state->Create(true, Factor::ONE, Factor::ONE, Operation::ADD, Factor::ONE, Factor::ONE, Operation::ADD); //비트 연산으로 key값 생성
		blendStates[key] = state;
	}
	state = nullptr;

	//Alpha
	state = new BlendState(context);
	{
		auto key = state->Create(true, Factor::SRC_ALPHA, Factor::INV_SRC_ALPHA, Operation::ADD, Factor::ONE, Factor::ONE, Operation::ADD); //비트 연산으로 key값 생성
		blendStates[key] = state;
	}
	state = nullptr;
}
