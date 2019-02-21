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

	void Render();
	void Clear();

private:
	void PassPreRender(); //Pass : IA~Rendering, PreRender : 처음에 그릴 것들 

private:
	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;

};