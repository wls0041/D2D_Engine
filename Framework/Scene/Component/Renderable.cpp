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
	SAFE_DELETE(mesh);
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

const BoundBox Renderable::GetBoundBox()
{
	boundBox = boundBox.Transformed(transform->GetWorldMatrix());
	return boundBox;
}

void Renderable::SetMaterial(const std::string & materialName)
{
	SAFE_DELETE(material);
	material = new Material(*(resourceMgr->Load<Material>(materialName)));
}

void Renderable::SetStandardMaterial()
{
	if (!material) material = new Material(context);
	material->SetShader("../../_Assets/Shader/Texture.hlsl");
}

void Renderable::SetMesh(const std::string & meshName)
{
	//mesh = resourceMgr->Load<Mesh>(meshName);
}

void Renderable::SetStandardMesh(const MeshType & type)
{
	if (!mesh) mesh = new Mesh(context);

	mesh->SetMeshType(type);
	boundBox = BoundBox(mesh->GetGeometry().GetVertices());
}
