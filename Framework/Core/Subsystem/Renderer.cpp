#include "Framework.h"
#include "Renderer.h"

Renderer::Renderer(Context * context)
{
	EventSystem::Get().Subscribe(EventType::Event_Render, EVENT_HANDLER(Render));
}

Renderer::~Renderer()
{
}

const bool Renderer::Initialize()
{
	return false;
}

ID3D11ShaderResourceView * Renderer::GetFrameResourceView() const
{
	return nullptr;
}

void Renderer::SetRenderables(Scene * scene)
{
}

void Renderer::Render()
{
}

void Renderer::Clear()
{
}

void Renderer::PassPreRender()
{
}
