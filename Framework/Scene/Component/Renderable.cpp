#include "Framework.h"
#include "Renderable.h"
#include "Transform.h"
#include "../GameObject.h"

Renderable::Renderable(Context * context, GameObject * object, Transform * transform) : IComponent(context, object, transform), material(nullptr), mesh(nullptr)
{
	resourceMgr = context->GetSubsystem<ResourceManager>();
}

Renderable::~Renderable()
{
	SAFE_DELETE(material);
}

void Renderable::OnInitialize()
{
}

void Renderable::OnStart()
{
}

void Renderable::OnUpdate()
{
}

void Renderable::OnStop()
{
}

void Renderable::OnDestroy()
{
}

void Renderable::SetMaterial(const std::string & materialName)
{
	SAFE_DELETE(material);
	material = new Material(*(resourceMgr->Load<Material>(materialName)));
}

void Renderable::SetMesh(const std::string & meshName)
{
	mesh = resourceMgr->Load<Mesh>(meshName);
}
