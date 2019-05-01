#pragma once
#include "ISubsystem.h"

enum class RenderableType : uint { OpaqueObject, TransparentObject, Camera, Light, Tilemap, }; //������, ����, ī�޶�

class Renderer final : public ISubsystem
{
public:
	Renderer(class Context *context);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	const bool Initialize() override;

	ID3D11ShaderResourceView * const GetFrameResourceView();
	ID3D11ShaderResourceView * const GetMainTarget();
	ID3D11ShaderResourceView * const GetBlur1Target();
	ID3D11ShaderResourceView * const GetBlur2Target();

	auto GetMainCamera() const-> class Camera*;
	void SetRenderables(class Scene *scene);
	
	auto GetViewport() const -> const D3D11_VIEWPORT& { return viewport; }
	void SetViewport(const D3D11_VIEWPORT &viewport) { this->viewport = viewport; }
	void SetViewport(const float &x, const float &y, const float &width, const float &height);

	auto GetResolution() const -> const Vector2& { return resolution; }
	void SetResolution(const uint &width, const uint &height);

	void AddLine(const Vector2 &from, const Vector2 &to, const Color &color);

	void Render();
	void Clear();

private:
	void CreateRenderTextures();
	void CreateShaders();

private:
	void PassTilemap();
	void PassObject(); //Pass : IA~Rendering, PreRender : ó���� �׸� �͵� 
	void PassLight();

	void PassBlur(std::shared_ptr<class RenderTexture>& in, std::shared_ptr<class RenderTexture>& out); //���� �޴� �κ��� ���
	void PassBloom(std::shared_ptr<class RenderTexture>& in, std::shared_ptr<class RenderTexture>& out); //�� ���� ó��

	void PassLine(std::shared_ptr<class RenderTexture>& out);

public:
	float bloomIntensity;
	float blurSigma;

private:
	Geometry<VertexColor> lines;
	std::shared_ptr<class VertexBuffer> lineVertexBuffer;
	uint line_count;

	D3D11_VIEWPORT viewport;
	Vector2 resolution;

private:
	class Camera *sceneCamera;
	std::shared_ptr<class Camera> editorCamera;

	std::shared_ptr<class VertexBuffer> screenVertexBuffer;
	std::shared_ptr<class IndexBuffer> screenIndexBuffer;

	std::shared_ptr<class Shader> lineShader;
	std::shared_ptr<class Shader> brightShader;
	std::shared_ptr<class Shader> blurShader;
	std::shared_ptr<class Shader> mergeShader;
	std::shared_ptr<class Shader> lightShader;
	std::shared_ptr<class Shader> blendShader;
	std::shared_ptr<class Shader> particleShader;

	std::shared_ptr<class ConstantBuffer> cameraBuffer;
	std::shared_ptr<class ConstantBuffer> transformBuffer;
	std::shared_ptr<class ConstantBuffer> blurBuffer;
	std::shared_ptr<class ConstantBuffer> lightBuffer;
	std::shared_ptr<class ConstantBuffer> tileBuffer;
	std::shared_ptr<class ConstantBuffer> tileColorBuffer;

	std::shared_ptr<class RenderTexture> mainTarget;
	std::shared_ptr<class RenderTexture> outputTarget;
	std::shared_ptr<class RenderTexture> blurTarget1;
	std::shared_ptr<class RenderTexture> blurTarget2;
	std::shared_ptr<class RenderTexture> lightTarget1;
	std::shared_ptr<class RenderTexture> lightTarget2;

	std::shared_ptr<class Pipeline> pipeline;

	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;
};