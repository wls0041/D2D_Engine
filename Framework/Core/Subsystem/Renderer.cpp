#include "Framework.h"
#include "Renderer.h"
#include "../D3D11/Pipeline.h"
#include "../../Scene/Scene.h"
#include "../../Scene/GameObject.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"

Renderer::Renderer(Context * context)
	: ISubsystem(context)
	, mainCamera(nullptr)
	, editorCamera(nullptr)
	, sceneCamera(nullptr)
	, mainTarget(nullptr)
	, cameraBuffer(nullptr)
	, transformBuffer(nullptr)
	, pipeline(nullptr)
{
	EventSystem::Get().Subscribe(EventType::Event_Render, EVENT_HANDLER(Render));
}

Renderer::~Renderer()
{
	SAFE_DELETE(editorCamera);
	SAFE_DELETE(mainTarget);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(transformBuffer);
	SAFE_DELETE(pipeline);
}

const bool Renderer::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();
	resourceMgr = context->GetSubsystem<ResourceManager>();

	editorCamera = new Camera(context);

	mainTarget = new RenderTexture(context);
	mainTarget->Create
	(
		static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight())
	);

	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	transformBuffer = new ConstantBuffer(context);
	transformBuffer->Create<WorldData>();

	pipeline = new Pipeline(context);

	return true;
}

ID3D11ShaderResourceView * Renderer::GetFrameResourceView() const
{
	return mainTarget->GetShaderResourceView();
}

void Renderer::SetRenderables(Scene * scene)
{
	Clear();

	auto objects = scene->GetObjects();
	for (auto object : objects)
	{
		auto renderable = object->GetComponent<Renderable>();
		auto camera = object->GetComponent<Camera>();

		if (renderable)
			renderables[RenderableType::OpaqueObject].emplace_back(object);
		else if (camera)
		{
			renderables[RenderableType::Camera].emplace_back(object);
			sceneCamera = camera;
		}
	}
}

void Renderer::Render()
{
	mainTarget->SetTarget();
	mainTarget->ClearTarget();

	mainCamera = Engine::IsOnEngineFlags(EngineFlags_Game) ? sceneCamera : editorCamera;

	if (mainCamera)
	{
		if (renderables.empty())
			return;

		mainCamera->UpdateEditorCamera();

		auto cameraData = cameraBuffer->Map<CameraData>();
		cameraData->View = mainCamera->GetViewMatrix();
		cameraData->Projection = mainCamera->GetProjectionMatrix();
		cameraBuffer->Unmap();

		pipeline->SetVSConstantBuffer(cameraBuffer);

		PassPreRender();
	}
}

void Renderer::Clear()
{
	renderables.clear();
	mainCamera = nullptr;
}

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

		pipeline->SetVertexBuffer(mesh->GetVertexBuffer());
		pipeline->SetIndexBuffer(mesh->GetIndexBuffer());
		pipeline->SetPrimitiveTopology(mesh->GetTopology());
		pipeline->SetInputLayout(material->GetInputLayout());
		pipeline->SetVertexShader(material->GetVertexShader());
		pipeline->SetPixelShader(material->GetPixelShader());
		pipeline->SetVSConstantBuffer(transformBuffer);
		pipeline->BindPipeline();

		pipeline->DrawIndexed(mesh->GetGeometry().GetIndexCount(), 0, 0);
	}
}
