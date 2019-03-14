#include "Framework.h"
#include "Renderer.h"
#include "../Pipeline.h"
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
		pipeline->BindPipeline();

		pipeline->DrawIndexed();
	}
}
