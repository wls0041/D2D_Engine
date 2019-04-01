#include "Framework.h"
#include "Renderer.h"
#include "../Pipeline.h"
#include "../../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/GameObject.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"

Renderer::Renderer(Context * context)
	: ISubsystem(context)
	, editorCamera(nullptr)
	, sceneCamera(nullptr)
	, mainTarget(nullptr)
	, outputTarget(nullptr)
	, blurTarget1(nullptr)
	, blurTarget2(nullptr)
	, brightShader(nullptr)
	, blurShader(nullptr)
	, mergeShader(nullptr)
	, cameraBuffer(nullptr)
	, transformBuffer(nullptr)
	, blurBuffer(nullptr)
	, pipeline(nullptr)
{
	EventSystem::Get().Subscribe(EventType::Event_Render, EVENT_HANDLER(Render));
}

Renderer::~Renderer()
{
	SAFE_DELETE(pipeline);
	SAFE_DELETE(mergeShader);
	SAFE_DELETE(blurShader);
	SAFE_DELETE(brightShader);
	SAFE_DELETE(blurTarget2);
	SAFE_DELETE(blurTarget1);
	SAFE_DELETE(outputTarget);
	SAFE_DELETE(mainTarget);
	SAFE_DELETE(blurBuffer);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(transformBuffer);
	SAFE_DELETE(editorCamera);
	SAFE_DELETE(screenIndexBuffer);
	SAFE_DELETE(screenVertexBuffer);
}

const bool Renderer::Initialize()
{
	Geometry<VertexTexture> geometry;
	GeometryUtility::CreateScreenQuad(geometry);

	screenVertexBuffer = new VertexBuffer(context);
	screenVertexBuffer->Create(geometry.GetVertices());
	
	screenIndexBuffer = new IndexBuffer(context);
	screenIndexBuffer->Create(geometry.GetIndices());

	editorCamera = new Camera(context);

	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	transformBuffer = new ConstantBuffer(context);
	transformBuffer->Create<WorldData>();
	
	blurBuffer = new ConstantBuffer(context);
	blurBuffer->Create<BlurData>();

	pipeline = new Pipeline(context);

	CreateRenderTextures();
	CreateShaders();

	return true;
}

ID3D11ShaderResourceView * Renderer::GetFrameResourceView() const
{
	return outputTarget->GetShaderResourceView();
}

auto Renderer::GetMainCamera() const -> Camera *
{
	return Engine::IsOnEngineFlags(EngineFlags_Game) ? sceneCamera : editorCamera;
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

	auto camera = GetMainCamera();

	if (camera)
	{
		if (renderables.empty())
			return;

		camera->UpdateEditorCamera();

		auto cameraData = cameraBuffer->Map<CameraData>();
		cameraData->View = camera->GetViewMatrix();
		cameraData->Projection = camera->GetProjectionMatrix();
		cameraBuffer->Unmap();

		PassPreRender();
		PassBloom(mainTarget, outputTarget);
	}
}

void Renderer::Clear()
{
	sceneCamera = nullptr;
	renderables.clear();
}

void Renderer::CreateRenderTextures()
{
	mainTarget = new RenderTexture(context);
	mainTarget->Create
	(
		static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight())
	);

	outputTarget = new RenderTexture(context);
	outputTarget->Create
	(
		static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight())
	);

	blurTarget1 = new RenderTexture(context);
	blurTarget1->Create
	(
		static_cast<uint>(Settings::Get().GetWidth() * 0.25f), //4분의 1로 만든 후 다시 퍼뜨려 더 퍼져보이게 만듬
		static_cast<uint>(Settings::Get().GetHeight() * 0.25f),
		DXGI_FORMAT_R16G16B16A16_FLOAT //색의 깊이를 높여 더 부드러운 색의 변화를 만듬
	);

	blurTarget2 = new RenderTexture(context);
	blurTarget2->Create
	(
		static_cast<uint>(Settings::Get().GetWidth() * 0.25f),
		static_cast<uint>(Settings::Get().GetHeight() * 0.25f),
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
}

void Renderer::CreateShaders()
{
	brightShader = new Shader(context);
	brightShader->AddDefine("PASS_BRIGHT");
	brightShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	brightShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	blurShader = new Shader(context);
	blurShader->AddDefine("PASS_GAUSSIANBLUR");
	blurShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	blurShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	mergeShader = new Shader(context);
	mergeShader->AddDefine("PASS_MERGE");
	mergeShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	mergeShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");
}

void Renderer::SwapRenderTarget(RenderTexture * lhs, RenderTexture * rhs)
{
	if (lhs->GetID() == rhs->GetID()) return;

	auto temp = lhs;
	lhs = rhs;
	rhs = temp;
}
