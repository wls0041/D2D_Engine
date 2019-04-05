#pragma once
#include "Framework.h"

template <class T>
class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

	const uint GetVertexCount() const { return vertices.size(); }
	const uint GetIndexCount() const { return indices.size(); }

	const uint GetVertexDataSize() const { return sizeof(T) * vertices.size(); }

	const std::vector<T>& GetVertices() const { return vertices; }
	void GetVertices
	(
		const uint& offset,
		const uint& count,
		std::vector<T>& vertices
	);

	const std::vector<uint>& GetIndices() const { return indices; }
	void GetIndices
	(
		const uint& offset,
		const uint& count,
		std::vector<uint>& indices
	);

	T* GetVertexData() { return vertices.data(); }
	uint* GetIndexData() { return indices.data(); }

	void Clear();

	void AddVertex(const T& vertex);
	void AddVertices(const std::vector<T>& vertices);
	void SetVertices(const std::vector<T>& vertice);

	void AddIndex(const uint& index);
	void AddIndices(const std::vector<uint>& indices);
	void Setindices(const std::vector<uint>& indices);

private:
	std::vector<T> vertices;
	std::vector<uint> indices;
};
#include "Geometry.inl"