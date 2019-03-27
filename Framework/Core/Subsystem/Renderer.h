#pragma once
#include "ISubsystem.h"

enum class RenderableType : uint { OpaqueObject, TransparentObject, Camera, }; //불투명, 투명, 카메라

class Renderer final : public ISubsystem
{
public:
	Renderer(class Context *context);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	const bool Initialize() override;

	ID3D11ShaderResourceView *GetFrameResourceView() const;
	auto GetMainCamera() const { return mainCamera; }
	void SetRenderables(class Scene *scene);
	
	void Render();
	void Clear();

private:
	void PassPreRender(); //Pass : IA~Rendering, PreRender : 처음에 그릴 것들 

	void PassBlur(class RenderTexture *in, class RenderTexture *out); //빛을 받는 부분을 떼어냄
	void PassBloom(class RenderTexture *in, class RenderTexture *out); //빛 번짐 처리

private:
	class Graphics *graphics;
	class ResourceManager *resourceMgr;

	class Camera *mainCamera;
	class Camera *editorCamera;
	class Camera *sceneCamera;

	class RenderTexture *mainTarget;
	class ConstantBuffer *cameraBuffer;
	class ConstantBuffer *transformBuffer;

	class Pipeline *pipeline;

	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;
};