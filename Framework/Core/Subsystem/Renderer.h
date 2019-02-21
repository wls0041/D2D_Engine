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

	void Render();
	void Clear();

private:
	void PassPreRender(); //Pass : IA~Rendering, PreRender : ó���� �׸� �͵� 

private:
	std::unordered_map<RenderableType, std::vector<class GameObject*>> renderables;

};