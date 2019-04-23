#pragma once

template <typename T> class Geometry; //template class의 전방선언

class GeometryUtility
{
public:
	static void CreateQuad(Geometry<struct VertexColor>& geometry);
	static void CreateQuad(Geometry<struct VertexTexture>& geometry);
	static void CreateTileQuad(Geometry<struct VertexTexture>& geometry);

	static void CreateScreenQuad(Geometry<struct VertexTexture>& geometry);
};