#pragma once

template <typename T> class Geometry; //template class�� ���漱��

class GeometryUtility
{
public:
	static void CreateQuad(Geometry<struct VertexColor>& geometry);
	static void CreateQuad(Geometry<struct VertexTexture>& geometry);
	static void CreateTileQuad(Geometry<struct VertexTexture>& geometry);

	static void CreateScreenQuad(Geometry<struct VertexTexture>& geometry);
};