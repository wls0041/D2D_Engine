#include "Geometry.h"

template<class T>
inline Geometry<T>::Geometry()
{
}

template<class T>
inline Geometry<T>::~Geometry()
{
}

template<class T>
inline void Geometry<T>::GetVertices(const uint & offset, const uint & count, std::vector<T>& vertices)
{
	auto first = this->vertices.begin() + offset;
	auto last = this->vertices.begin() + offset + count;

	vertices.assign(first, last);
}

template<class T>
inline void Geometry<T>::GetIndices(const uint & offset, const uint & count, std::vector<uint>& indices)
{
	auto first = this->indices.begin() + offset;
	auto last = this->indices.begin() + offset + count;

	indices.assign(first, last);
}

template<class T>
inline void Geometry<T>::Clear()
{
	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();
}

template<class T>
inline void Geometry<T>::AddVertex(const T & vertex)
{
	vertices.emplace_back(vertex);
}

template<class T>
inline void Geometry<T>::AddVertices(const std::vector<T>& vertices)
{
	this->vertices.insert
	(
		this->vertices.end(),
		vertices.begin(),
		vertices.end()
	);
}

template<class T>
inline void Geometry<T>::SetVertices(const std::vector<T>& vertice)
{
	Clear();
	this->vertices.assign
	(
		vertices.begin(),
		vertices.end()
	);
}

template<class T>
inline void Geometry<T>::AddIndex(const uint & index)
{
	indices.emplace_back(index);
}

template<class T>
inline void Geometry<T>::AddIndices(const std::vector<uint>& indices)
{
	this->indices.insert
	(
		this->indices.end(),
		indices.begin(),
		indices.end()
	);
}

template<class T>
inline void Geometry<T>::Setindices(const std::vector<uint>& indices)
{
	Clear();
	this->indices.assign
	(
		indices.begin(),
		indices.end()
	);
}
