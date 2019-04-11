#include "Framework.h"
#include "Renderer.h"
#include "../Pipeline.h"
#include "../../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/GameObject.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"
#include "../../Scene/Component/Light.h"
#include "../../Scene/Component/ParticleEmitter.h"

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
		pipeline->SetBlendState(BlendMode::Blend_None);
		pipeline->BindPipeline();

		pipeline->DrawIndexed();
		////////////////////임시////////////////////////
		auto particleEmitter = object->GetComponent<ParticleEmitter>();
		auto resourceMgr = context->GetSubsystem<ResourceManager>();

		if (!particleEmitter) return;

		pipeline->SetVertexBuffer(particleEmitter->GetVertexBuffer());
		pipeline->SetIndexBuffer(particleEmitter->GetIndexBuffer());
		pipeline->SetPrimitiveTopology(mesh->GetTopology());
		pipeline->SetInputLayout(particleShader->GetInputLayout());
		pipeline->SetVertexShader(particleShader->GetVertexShader());
		pipeline->SetPixelShader(particleShader->GetPixelShader());
		pipeline->SetVSConstantBuffer(cameraBuffer);
		pipeline->SetVSConstantBuffer(transformBuffer);
		pipeline->SetPSShaderResource(resourceMgr->Load<Texture>("Flame.jpg"));
		pipeline->SetBlendState(BlendMode::Blend_Alpha);
		//pipeline->SetBlendState(BlendMode::Blend_None);

		pipeline->BindPipeline();

		pipeline->DrawIndexed();
	}
}

void Renderer::PassLight()
{
	LightData lightData;
	uint index = 0;
	for (const auto &object : renderables[RenderableType::Light]) {
		auto light = object->GetComponent<Light>();
		auto transform = object->GetTransform();

		lightData.Datas[index].LightColor = light->GetColor();
		lightData.Datas[index].LightMaxDist = light->GetMaxDistance();
		lightData.Datas[index].LightMinDist = light->GetMinDistance();
		lightData.Datas[index].LightPos = transform->GetPosition();
		lightData.Datas[index].LightDistFactor = light->GetDistanceFactor();
		lightData.Datas[index].IsTwinkle = light->IsTwinkle() ? 1 : 0;
		lightData.Datas[index].Time = context->GetSubsystem<Timer>()->GetDeltaTimeMs();

		index++;
	}

	auto data = lightBuffer->Map<LightData>();
	{
		memcpy(data, &lightData, sizeof(LightData));
	}
	lightBuffer->Unmap();

	auto worldData = transformBuffer->Map<WorldData>();
	{
		worldData->World = Matrix::Identity;
	}
	transformBuffer->Unmap();

	lightTarget1->SetTarget();
	lightTarget1->ClearTarget();

	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(lightShader->GetInputLayout());
	pipeline->SetVertexShader(lightShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(lightShader->GetPixelShader());
	pipeline->SetPSConstantBuffer(lightBuffer);
	pipeline->SetBlendState(BlendMode::Blend_None);
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

	lightTarget2->SetTarget();
	lightTarget2->ClearTarget();

	//임시로 merge 추후 blend로
	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(mergeShader->GetInputLayout());
	pipeline->SetVertexShader(mergeShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(mergeShader->GetPixelShader());
	pipeline->SetPSShaderResource(mainTarget->GetShaderResourceView());
	pipeline->SetPSShaderResource(lightTarget1->GetShaderResourceView());
	pipeline->SetBlendState(BlendMode::Blend_None);
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

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
	pipeline->SetBlendState(BlendMode::Blend_None);
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
	pipeline->SetBlendState(BlendMode::Blend_None);
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
	pipeline->SetBlendState(BlendMode::Blend_None);
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
	pipeline->SetBlendState(BlendMode::Blend_None);
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

	//==================================blend======================================
	lightTarget2->SetTarget();
	lightTarget2->ClearTarget();

	pipeline->SetVertexBuffer(screenVertexBuffer);
	pipeline->SetIndexBuffer(screenIndexBuffer);
	pipeline->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->SetInputLayout(blendShader ->GetInputLayout());
	pipeline->SetVertexShader(blendShader->GetVertexShader());
	pipeline->SetVSConstantBuffer(cameraBuffer);
	pipeline->SetVSConstantBuffer(transformBuffer);
	pipeline->SetPixelShader(blendShader->GetPixelShader());
	pipeline->SetPSShaderResource(mainTarget->GetShaderResourceView());
	pipeline->SetPSShaderResource(lightTarget1->GetShaderResourceView());
	pipeline->SetBlendState(BlendMode::Blend_None);
	pipeline->BindPipeline();

	pipeline->DrawIndexed();

}
