#include "Framework.h"
#include "GeometryUtility.h"

void GeometryUtility::CreateQuad(Geometry<VertexColor>& geometry)
{
	geometry.AddVertex(VertexColor(Vector3(-0.5f, -0.5f, 0.0f), Color(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(Vector3(-0.5f, +0.5f, 0.0f), Color(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(Vector3(+0.5f, -0.5f, 0.0f), Color(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(Vector3(+0.5f, +0.5f, 0.0f), Color(0, 0, 0, 1)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateQuad(Geometry<VertexTexture>& geometry)
{
	geometry.AddVertex(VertexTexture(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0, 1)));
	geometry.AddVertex(VertexTexture(Vector3(-0.5f, +0.5f, 0.0f), Vector2(0, 0)));
	geometry.AddVertex(VertexTexture(Vector3(+0.5f, -0.5f, 0.0f), Vector2(1, 1)));
	geometry.AddVertex(VertexTexture(Vector3(+0.5f, +0.5f, 0.0f), Vector2(1, 0)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateTileQuad(Geometry<struct VertexTexture>& geometry)
{
	geometry.AddVertex(VertexTexture(Vector3(0.0f, 0.0f, 0.0f), Vector2(0, 1)));
	geometry.AddVertex(VertexTexture(Vector3(0.0f, 1.0f, 0.0f), Vector2(0, 0)));
	geometry.AddVertex(VertexTexture(Vector3(1.0f, 0.0f, 0.0f), Vector2(1, 1)));
	geometry.AddVertex(VertexTexture(Vector3(1.0f, 1.0f, 0.0f), Vector2(1, 0)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateScreenQuad(Geometry<VertexTexture>& geometry)
{
	float w = Settings::Get().GetWidth() * 0.5f;
	float h = Settings::Get().GetHeight() * 0.5f;

	geometry.AddVertex(VertexTexture(Vector3(-w, -h, 0.0f), Vector2(0, 1)));
	geometry.AddVertex(VertexTexture(Vector3(-w, +h, 0.0f), Vector2(0, 0)));
	geometry.AddVertex(VertexTexture(Vector3(+w, -h, 0.0f), Vector2(1, 1)));
	geometry.AddVertex(VertexTexture(Vector3(+w, +h, 0.0f), Vector2(1, 0)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateScreenQuad(Geometry<struct VertexTexture>& geometry, const uint & width, const uint & height)
{
	if (width == 0 || height == 0) {
		LOG_ERROR("Invalid parameter");
		return;
	}

	float half_w = static_cast<float>(width);
	float half_h = static_cast<float>(height);

	geometry.AddVertex(VertexTexture(Vector3(-half_w, -half_h, 0.0f), Vector2(0, 1)));
	geometry.AddVertex(VertexTexture(Vector3(-half_w, +half_h, 0.0f), Vector2(0, 0)));
	geometry.AddVertex(VertexTexture(Vector3(+half_w, -half_h, 0.0f), Vector2(1, 1)));
	geometry.AddVertex(VertexTexture(Vector3(+half_w, +half_h, 0.0f), Vector2(1, 0)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}
