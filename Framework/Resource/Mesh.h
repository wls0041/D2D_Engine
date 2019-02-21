#pragma once
#include "IResource.h"

enum class MeshType : uint { Unknown, Quad, ScreenQuad, Custom};

class Mesh final : public IResource
{
public:
	Mesh(class Context *context);
	~Mesh();

	//Mesh() = default; //Mesh() {}
	Mesh(const Mesh &rhs) = delete; //복사생성자를 사용하지 않겠다. 포인터만 넘겨주고 싶은데 복사생성자 호출되어 새로 할당되는 걸 방지
	Mesh &operator=(const Mesh &rhs) = delete;

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	const class Geometry<VertexTexture> &GetGeometry() const { return geometry; }
	class VertexBuffer *GetVertexBuffer() const { return vertexBuffer; }
	class IndexBuffer* GetIndexBuffer() const { return indexBuffer; }
	const D3D11_PRIMITIVE_TOPOLOGY &GetTopology() const { return topology; }

	void SetTopology(const D3D11_PRIMITIVE_TOPOLOGY &topology) { this->topology = topology; }
	void SetMeshType(const MeshType &meshType);

private:
	class Geometry<VertexTexture> geometry;
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	MeshType meshType;
};