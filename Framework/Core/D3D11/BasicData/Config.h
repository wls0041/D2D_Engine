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

struct BlurData
{
	Vector2 BlurDirection;
	Vector2 BlurTexelSize;
	float BlurSigma;
	float Padding[3];
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

#define MAXLIGHT 64
struct LightData
{
	struct Data {
		Color LightColor;
		Vector3 LightPos;
		float LightMinDist;
		float LightMaxDist;
		float LightDistFactor;
		float Time;
		int IsTwinkle;
	} Datas[MAXLIGHT];
};