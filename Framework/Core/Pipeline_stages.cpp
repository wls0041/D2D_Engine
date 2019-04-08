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
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, vs_constantBuffers.size(), vs_constantBuffers.data()); //3����. const ������� -> �������� �迭�� �ްڴ�(�����͵��� ������)

		vs_constantBuffers.clear(); //���� �Ϸ� �� ����
		vs_constantBuffers.shrink_to_fit();
	}

	//shaderResource
	if (!vs_shaderResources.empty())
	{
		graphics->GetDeviceContext()->VSSetShaderResources(0, vs_shaderResources.size(), vs_shaderResources.data());

		vs_shaderResources.clear(); //���� �Ϸ� �� ����
		vs_shaderResources.shrink_to_fit();
	}

	//sampler
	if (!vs_samplers.empty())
	{
		graphics->GetDeviceContext()->VSSetSamplers(0, vs_samplers.size(), vs_samplers.data());

		vs_samplers.clear(); //���� �Ϸ� �� ����
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
		graphics->GetDeviceContext()->PSSetConstantBuffers(0, ps_constantBuffers.size(), ps_constantBuffers.data()); //3����. const ������� -> �������� �迭�� �ްڴ�(�����͵��� ������)

		ps_constantBuffers.clear(); //���� �Ϸ� �� ����
		ps_constantBuffers.shrink_to_fit();
	}

	//shaderResource
	if (!ps_shaderResources.empty())
	{
		graphics->GetDeviceContext()->PSSetShaderResources(0, ps_shaderResources.size(), ps_shaderResources.data());

		ps_shaderResources.clear(); //���� �Ϸ� �� ����
		ps_shaderResources.shrink_to_fit();
	}

	//sampler
	if (!ps_samplers.empty())
	{
		graphics->GetDeviceContext()->PSSetSamplers(0, ps_samplers.size(), ps_samplers.data());

		ps_samplers.clear(); //���� �Ϸ� �� ����
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
		auto key = state->Create(true, Factor::ONE, Factor::ONE, Operation::ADD, Factor::ONE, Factor::ONE, Operation::ADD); //��Ʈ �������� key�� ����
		blendStates[key] = state;
	}
	state = nullptr;

	//Alpha
	state = new BlendState(context);
	{
		auto key = state->Create(true, Factor::SRC_ALPHA, Factor::INV_SRC_ALPHA, Operation::ADD, Factor::ONE, Factor::ONE, Operation::ADD); //��Ʈ �������� key�� ����
		blendStates[key] = state;
	}
	state = nullptr;
}
