#pragma once
#include "ISubsystem.h"

enum class RenderableType : uint { OpaqueObject, TransparentObject, Camera, Light, Tilemap, }; //불투명, 투명, 카메라

class Renderer final : public ISubsystem
{
public:
	Renderer(class Context *context);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	const bool Initialize() override;

	ID3D11ShaderResourceView *GetFrameResourceView() const;
	ID3D11ShaderResourceView *GetMainTarget() const;
	ID3D11ShaderResourceView *GetBlur1Target() const;
	ID3D11ShaderResourceView *GetBlur2Target() const;

	auto GetMainCamera() const-> class Camera*;
	void SetRenderables(class Scene *scene);
	
	void Render();
	void Clear();

private:
	void CreateRenderTextures();
	void CreateShaders();

private:
	void PassTilemap();
	void PassObject(); //Pass : IA~Rendering, PreRender : 처음에 그릴 것들 
	void PassLight();

	void PassBlur(class RenderTexture *in, class RenderTexture *out); //빛을 받는 부분을 떼어냄
	void PassBloom(class RenderTexture *in, class RenderTexture *out); //빛 번짐 처리

private:
	void SwapRenderTarget(class RenderTexture *lhs, class RenderTexture *rhs);

public:
	float bloomIntensity;
	float blurSigma;

private:
	class Geometry<VertexTexture> screenGeometry;
	class VertexBuffer *screenVertexBuffer;
	class IndexBuffer *screenIndexBuffer;

	class Camera *editorCamera;
	class Camera *sceneCamera;

	class Shader *brightShader;
	class Shader *blurShader;
	class Shader *mergeShader;
	class Shader *lightShader;
	class Shader *blendShader;
	class Shader *particleShader;

	class ConstantBuffer *cameraBuffer;
	class ConstantBuffer *transformBuffer;
	class ConstantBuffer *blurBuffer;
	class ConstantBuffer *lightBuffer;
	class ConstantBuffer *tileBuffer;

	class RenderTexture *mainTarget;
	class RenderTexture *outputTarget;
	class RenderTexture *blurTarget1;
	class RenderTexture *blurTarget2;
	class RenderTexture *lightTarget1;
	class RenderTexture *lightTarget2;

	class Pipeline *pipeline;

	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;
};