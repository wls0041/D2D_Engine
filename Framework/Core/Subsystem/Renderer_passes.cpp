#include "Framework.h"
#include "Renderer.h"
#include "../Pipeline.h"
#include "../../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/GameObject.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"

void Renderer::PassPreRender()
{
	for (auto object : renderables[RenderableType::OpaqueObject])
	{
		auto renderable = object->GetComponent<Renderable>();
		auto transform = object->GetTransform();

		auto material = renderable->GetMaterial();
		auto mesh = renderable->GetMesh();

		if (!material || !mesh)
		{
			Log::Write("Could not find mesh or material", LogType::Warning);
			continue;
		}

		auto worldData = transformBuffer->Map<WorldData>();
		{
			worldData->World = transform->GetWorldMatrix();
		}
		transformBuffer->Unmap();

		pipeline->SetVertexBuffer(mesh->GetVertexBuffer());
		pipeline->SetIndexBuffer(mesh->GetIndexBuffer());
		pipeline->SetPrimitiveTopology(mesh->GetTopology());
		pipeline->SetInputLayout(material->GetInputLayout());
		pipeline->SetVertexShader(material->GetVertexShader());
		pipeline->SetPixelShader(material->GetPixelShader());
		pipeline->SetVSConstantBuffer(cameraBuffer);
		pipeline->SetVSConstantBuffer(transformBuffer);
		pipeline->SetPSShaderResource(material->GetDiffuseTexture());
		pipeline->BindPipeline();

		pipeline->DrawIndexed();
	}
}

void Renderer::PassBlur(RenderTexture * in, RenderTexture * out)
{
	bool bCheck = false; //두 텍스쳐의 사이즈와 포맷이 같아야 연산 가능
	bCheck |= in->GetWidth() != out->GetWidth();
	bCheck |= in->GetHeight() != out->GetHeight();
	bCheck |= in->GetFormat() != out->GetFormat();

	if (bCheck) {
		Log::Error("Renderer::PassBlur : Textures are not match");
		return;
	}

	auto worldData = transformBuffer->Map<WorldData>();
	{
		worldData->World = Matrix::Identity;
	}
	transformBuffer->Unmap();

	//=================================BlurX=======================================
	auto blurXData = blurBuffer->Map<BlurData>();
	{
		blurXData->BlurDirection = Vector2(1.0f, 0.0f);
		blurXData->BlurTexelSize = Vector2(1.0f / out->GetWidth(), 1.0f / out->GetHeight());
		blurXData->BlurSigma = 2.0f;
	}
	blurBuffer->Unmap();

	out->SetTarget();
	out->ClearTarget();
	
	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(blurShader->GetInputLayout());
	pipeline->SetVertexShader(blurShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(blurShader->GetPixelShader());
	pipeline->SetPSConstantBuffer(blurBuffer);
	pipeline->SetPSShaderResource(in->GetShaderResourceView());
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

	//=================================BlurY=======================================
	auto blurYData = blurBuffer->Map<BlurData>();
	{
		blurYData->BlurDirection = Vector2(0.0f, 1.0f);
		blurYData->BlurTexelSize = Vector2(1.0f / in->GetWidth(), 1.0f / in->GetHeight());
		blurYData->BlurSigma = 2.0f;
	}
	blurBuffer->Unmap();

	in->SetTarget();
	in->ClearTarget();

	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(blurShader->GetInputLayout());
	pipeline->SetVertexShader(blurShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(blurShader->GetPixelShader());
	pipeline->SetPSConstantBuffer(blurBuffer);
	pipeline->SetPSShaderResource(out->GetShaderResourceView());
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

	SwapRenderTarget(in, out);
}

void Renderer::PassBloom(RenderTexture * in, RenderTexture * out)
{
	bool bCheck = false; 
	bCheck |= in->GetWidth() != out->GetWidth();
	bCheck |= in->GetHeight() != out->GetHeight();
	bCheck |= in->GetFormat() != out->GetFormat();

	if (bCheck) {
		Log::Error("Renderer::PassBlur : Textures are not match");
		return;
	}

	auto worldData = transformBuffer->Map<WorldData>();
	{
		worldData->World = Matrix::Identity;
	}
	transformBuffer->Unmap();


	//=================================Bright======================================
	blurTarget1->SetTarget();
	blurTarget1->ClearTarget();

	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(brightShader->GetInputLayout());
	pipeline->SetVertexShader(brightShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(brightShader->GetPixelShader());
	pipeline->SetPSShaderResource(in->GetShaderResourceView());
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

	//==============================Gaussian Blur==================================
	PassBlur(blurTarget1, blurTarget2);

	//==================================Merge======================================
	out->SetTarget();
	out->ClearTarget();

	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(mergeShader->GetInputLayout());
	pipeline->SetVertexShader(mergeShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(mergeShader->GetPixelShader());
	pipeline->SetPSShaderResource(in->GetShaderResourceView());
	pipeline->SetPSShaderResource(blurTarget2->GetShaderResourceView());
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

}
