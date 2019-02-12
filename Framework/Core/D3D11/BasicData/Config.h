#pragma once
#include "Framework.h"

enum class ShaderType : uint
{
	VS, PS, GS, CS,
};

struct CameraData
{
	Matrix View;
	Matrix Projection;
};

struct WorldData
{
	Matrix World;
};

struct ColorData
{
	Color Color;
};

struct AnimationData
{
	Vector2 TextureSize;
	Vector2 SpriteOffset;
	Vector2 SpriteSize;
	float Padding[2];
};