#pragma once
#include "IResource.h"

enum class MeshType : uint { Unknown, Quad, ScreenQuad, Custom};

class Mesh final : public IResource
{
public:
	Mesh(class Context *context);
	~Mesh();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	const class Geometry<VertexTexture> &GetGeometry() const { return geometry; }
	class VertexBuffer *GetVertexBuffer() const { return vertexBuffer; }
	class IndexBuffer* GetIndexBuffer() const { return indexBuffer; }
	const D3D11_PRIMITIVE &GetTopology() const { return topology; }

	void SetTopology(const D3D11_PRIMITIVE &topology) { this->topology = topology; }
	void SetMeshType(const MeshType &meshType);

private:
	class Geometry<VertexTexture> geometry;
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	MeshType meshType;
	D3D11_PRIMITIVE topology;
};