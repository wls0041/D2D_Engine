#pragma once
#include "Framework.h"

enum class EmitterType : uint { Gravity, Radius, };
enum class PositionType : uint { Free, Parent, Group, };

//particle 집합정보
struct ParticleData final
{
	ParticleData() { ZeroMemory(this, sizeof(ParticleData)); }
	
	std::string name;

	uint blendFuncSource;
	uint blendFuncDestination;
	
	EmitterType emitterType;
	
	uint maxParticles;

	float duration;
	float particleLifeSpan;
	float particleLifeSpanVariance;

	float speed;
	float speedVariance;

	Vector2 sourcePosition;
	Vector2 sourcePositionVariance;
	
	PositionType positionType;

	float startParticleSize;
	float startParticleSizeVariance;
	
	float finishParticleSize;
	float finishParticleSizeVariance;

	float angle;
	float angleVariance;

	float startRotation;
	float startRotationVariance;

	float finishRotation;
	float finishRotationVariance;

	float rotationPerSecond;
	float rotationPerSecondVariance;

	float minRadius;
	float minRadiusVariance;

	float maxRadius;
	float maxRadiusVariance;

	float radialAccelation;
	float radialAccelationVariance;

	float tangentialAccelation;
	float tangentialAccelationVariance;

	bool bAbsolutePosition;
	bool bYCoordFlipped;
	bool bRotationIsDirection;

	Vector2 gravity;

	float startColorR;
	float startColorG;
	float startColorB;
	float startColorA;
	
	float startColorRVariance;
	float startColorGVariance;
	float startColorBVariance;
	float startColorAVariance;

	float finishColorR;
	float finishColorG;
	float finishColorB;
	float finishColorA;
	
	float finishColorRVariance;
	float finishColorGVariance;
	float finishColorBVariance;
	float finishColorAVariance;

	float emissionRate;

	Texture *texture;
};