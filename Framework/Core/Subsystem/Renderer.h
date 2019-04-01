#pragma once
#include "ISubsystem.h"

enum class RenderableType : uint { OpaqueObject, TransparentObject, Camera, }; //������, ����, ī�޶�

class Renderer final : public ISubsystem
{
public:
	Renderer(class Context *context);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	const bool Initialize() override;

	ID3D11ShaderResourceView *GetFrameResourceView() const;
	auto GetMainCamera() const-> class Camera*;
	void SetRenderables(class Scene *scene);
	
	void Render();
	void Clear();

private:
	void CreateRenderTextures();
	void CreateShaders();

private:
	void PassPreRender(); //Pass : IA~Rendering, PreRender : ó���� �׸� �͵� 

	void PassBlur(class RenderTexture *in, class RenderTexture *out); //���� �޴� �κ��� ���
	void PassBloom(class RenderTexture *in, class RenderTexture *out); //�� ���� ó��

private:
	void SwapRenderTarget(class RenderTexture *lhs, class RenderTexture *rhs);

private:
	class Geometry<VertexTexture> screenGeometry;
	class VertexBuffer *screenVertexBuffer;
	class IndexBuffer *screenIndexBuffer;

	class Camera *editorCamera;
	class Camera *sceneCamera;

	class Shader *brightShader;
	class Shader *blurShader;
	class Shader *mergeShader;

	class RenderTexture *mainTarget;
	class RenderTexture *outputTarget;
	class RenderTexture *blurTarget1;
	class RenderTexture *blurTarget2;

	class ConstantBuffer *cameraBuffer;
	class ConstantBuffer *transformBuffer;
	class ConstantBuffer *blurBuffer;

	class Pipeline *pipeline;

	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;
};