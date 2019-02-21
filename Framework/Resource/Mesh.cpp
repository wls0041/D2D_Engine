#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Context * context) : IResource(context), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), meshType(MeshType::Unknown)
{
	vertexBuffer = new VertexBuffer(context);
	indexBuffer = new IndexBuffer(context);
}

Mesh::~Mesh() 
{               
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Mesh::SaveToFile(const std::string & filePath)
{
}

void Mesh::LoadFromFile(const std::string & filePath)
{
}

void Mesh::SetMeshType(const MeshType & meshType)
{
	if (this->meshType == meshType) return;

	this->meshType = meshType;

	geometry.Clear();
	vertexBuffer->Clear();
	indexBuffer->Clear();

	switch (meshType)
	{
	case MeshType::Quad:
		GeometryUtility::CreateQuad(geometry);
		break;
	case MeshType::ScreenQuad:
		GeometryUtility::CreateScreenQuad(geometry);
		break;
	case MeshType::Custom:
		break;
	}

	vertexBuffer->Create(geometry.GetVertices());
	indexBuffer->Create(geometry.GetIndices());
}
