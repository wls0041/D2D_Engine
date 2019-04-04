#pragma once

struct VertexColor
{
	VertexColor()
		: position(0, 0, 0)
		, color(0, 0, 0, 1)
	{}

	VertexColor
	(
		const Vector3& position,
		const Color& color
	)
		: position(position)
		, color(color)
	{}

	Vector3 position;
	Color color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

struct VertexTexture
{
	VertexTexture()
		: position(0, 0, 0)
		, uv(0, 0)
	{}

	VertexTexture
	(
		const Vector3& position,
		const Vector2& uv
	)
		: position(position)
		, uv(uv)
	{}

	Vector3 position;
	Vector2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

struct VertexTextureColor
{
	VertexTextureColor()
		: position(0, 0, 0)
		, uv(0, 0)
		, color(0, 0, 0, 0)
	{}

	VertexTextureColor
	(
		const Vector3& position,
		const Vector2& uv,
		const Color &color
	)
		: position(position)
		, uv(uv)
		, color(color)
	{}

	Vector3 position;
	Vector2 uv;
	Color color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};