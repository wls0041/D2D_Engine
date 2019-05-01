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
#include "../../Scene/Component/Tilemap.h"

Renderer::Renderer(Context * context)
	: ISubsystem(context)
	, sceneCamera(nullptr)
	, bloomIntensity(0.4f)
	, blurSigma(1.0f)
{
	//viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Settings::Get().GetWidth();
	viewport.Height = Settings::Get().GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//resolution
	resolution = Vector2(viewport.Width, viewport.Height);

	EventSystem::Get().Subscribe(EventType::Event_Render, EVENT_HANDLER(Render));
}

Renderer::~Renderer()
{
}

const bool Renderer::Initialize()
{
	lineVertexBuffer = std::make_shared<VertexBuffer>(context);

	editorCamera = std::make_shared<Camera>(context);

	cameraBuffer = std::make_shared<ConstantBuffer>(context);
	cameraBuffer->Create<CameraData>();

	transformBuffer = std::make_shared<ConstantBuffer>(context);
	transformBuffer->Create<WorldData>();

	blurBuffer = std::make_shared<ConstantBuffer>(context);
	blurBuffer->Create<BlurData>();

	lightBuffer = std::make_shared<ConstantBuffer>(context);
	lightBuffer->Create<LightData>();

	tileBuffer = std::make_shared<ConstantBuffer>(context);
	tileBuffer->Create<TileData>();

	tileColorBuffer = std::make_shared<ConstantBuffer>(context);
	tileColorBuffer->Create<ColorData>();

	pipeline = std::make_shared<Pipeline>(context);

	CreateRenderTextures();
	CreateShaders();

	return true;
}

ID3D11ShaderResourceView * const Renderer::GetFrameResourceView()
{
	return mainTarget->GetShaderResourceView();
}

ID3D11ShaderResourceView * const Renderer::GetMainTarget()
{
	return mainTarget->GetShaderResourceView();
}

ID3D11ShaderResourceView * const Renderer::GetBlur1Target()
{
	return blurTarget1->GetShaderResourceView();
}

ID3D11ShaderResourceView * const Renderer::GetBlur2Target()
{
	return blurTarget2->GetShaderResourceView();
}

auto Renderer::GetMainCamera() const -> Camera *
{
	//raw pointer
	return Engine::IsOnEngineFlags(EngineFlags_Game) ? sceneCamera : editorCamera.get();
}

void Renderer::SetRenderables(Scene * scene)
{
	Clear();

	auto objects = scene->GetObjects();
	for (auto object : objects)
	{
		auto renderable = object->GetComponent<Renderable>();
		auto camera = object->GetComponent<Camera>();
		auto light = object->GetComponent<Light>();
		auto tilemap = object->GetComponent<Tilemap>();

		if (renderable) renderables[RenderableType::OpaqueObject].emplace_back(object);
		else if (camera)
		{
			renderables[RenderableType::Camera].emplace_back(object);
			sceneCamera = camera;
		}
		else if (light) renderables[RenderableType::Light].emplace_back(object);
		else if (tilemap) renderables[RenderableType::Tilemap].emplace_back(object);
	}
}

void Renderer::SetViewport(const float & x, const float & y, const float & width, const float & height)
{
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void Renderer::SetResolution(const uint & width, const uint & height)
{
	if (width == 0 || height == 0) {
		LOG_FERROR("%dx%d is an invalid resolution", width, height);
		return;
	}

	if (resolution.x == width && resolution.y == height) return;

	resolution.x = static_cast<float>((width % 2 != 0) ? width - 1 : width);
	resolution.y = static_cast<float>((height % 2 != 0) ? height - 1 : height);

	CreateRenderTextures();

	LOG_FINFO("Resolution set to %dx%d", width, height);
}

void Renderer::AddLine(const Vector2 & from, const Vector2 & to, const Color & color)
{
	lines.AddVertex(VertexColor(Vector3(from), color));
	lines.AddVertex(VertexColor(Vector3(to), color));
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

		PassTilemap();
		PassObject();
		PassLine(mainTarget);
		//PassLight();
		//PassBloom(mainTarget, outputTarget);
	}
}

void Renderer::Clear()
{
	sceneCamera = nullptr;
	renderables.clear();
}

void Renderer::CreateRenderTextures()
{
	auto width = static_cast<uint>(resolution.x);
	auto height = static_cast<uint>(resolution.y);
	
	Geometry<VertexTexture> geometry;
	GeometryUtility::CreateScreenQuad(geometry, width, height);

	screenVertexBuffer = std::make_shared<VertexBuffer>(context);
	screenVertexBuffer->Create(geometry.GetVertices());

	screenIndexBuffer = std::make_shared<IndexBuffer>(context);
	screenIndexBuffer->Create(geometry.GetIndices());

	mainTarget = std::make_shared<RenderTexture>(context);
	mainTarget->Create(width, height);

	outputTarget = std::make_shared<RenderTexture>(context);
	outputTarget->Create(width, height);

	lightTarget1 = std::make_shared<RenderTexture>(context);
	lightTarget1->Create(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	lightTarget2 = std::make_shared<RenderTexture>(context);
	lightTarget2->Create(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	blurTarget1 = std::make_shared<RenderTexture>(context);
	blurTarget1->Create
	(
		static_cast<uint>(width * 0.25f), //4분의 1로 만든 후 다시 퍼뜨려 더 퍼져보이게 만듬
		static_cast<uint>(height * 0.25f),
		DXGI_FORMAT_R16G16B16A16_FLOAT //색의 깊이를 높여 더 부드러운 색의 변화를 만듬
	);

	blurTarget2 = std::make_shared<RenderTexture>(context);
	blurTarget2->Create(static_cast<uint>(width * 0.25f), static_cast<uint>(height* 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT);

	//bpp - byte per pixel = 4
	//bpc - byte per color -> color depth = 1
}

void Renderer::CreateShaders()
{
	lineShader = std::make_shared<Shader>(context);
	lineShader->AddShader(ShaderType::VS, "../../_Assets/Shader/line.hlsl");
	lineShader->AddShader(ShaderType::PS, "../../_Assets/Shader/line.hlsl");

	brightShader = std::make_shared<Shader>(context);
	brightShader->AddDefine("PASS_BRIGHT");
	brightShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	brightShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	blurShader = std::make_shared<Shader>(context);
	blurShader->AddDefine("PASS_GAUSSIANBLUR");
	blurShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	blurShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	mergeShader = std::make_shared<Shader>(context);
	mergeShader->AddDefine("PASS_MERGE");
	mergeShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	mergeShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	blendShader = std::make_shared<Shader>(context);
	mergeShader->AddDefine("PASS_BLEND");
	blendShader->AddShader(ShaderType::VS, "../../_Assets/Shader/PostEffect.hlsl");
	blendShader->AddShader(ShaderType::PS, "../../_Assets/Shader/PostEffect.hlsl");

	lightShader = std::make_shared<Shader>(context);
	lightShader->AddShader(ShaderType::VS, "../../_Assets/Shader/Light.hlsl");
	lightShader->AddShader(ShaderType::PS, "../../_Assets/Shader/Light.hlsl");

	particleShader = std::make_shared<Shader>(context);
	particleShader->AddShader(ShaderType::VS, "../../_Assets/Shader/Particle.hlsl");
	particleShader->AddShader(ShaderType::PS, "../../_Assets/Shader/Particle.hlsl");
}

