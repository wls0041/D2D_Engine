#pragma once
#include "Framework.h"

//paricle 개별정보
struct Particle final
{
	Particle() { ZeroMemory(this, sizeof(Particle)); }
	
	float life;

	float angle;
	float speed;

	float size;
	float deltaSize;

	float rotation;
	float deltaRotation;

	float radialAccelation;
	float tangentialAccelation;

	float radius;
	float deltaRadius;
	float degreePerSecond;

	float colorR;
	float colorG;
	float colorB;
	float colorA;

	float deltaColorR;
	float deltaColorG;
	float deltaColorB;
	float deltaColorA;

	Vector2 direction;
	Vector2 position;
};