#pragma once
#include "IComponent.h"
#include "../../Particle/Particle.h"
#include "../../Particle/ParticleData.h"

class ParticleEmitter final : public IComponent
{
public:
	ParticleEmitter(class Context *context, class GameObject *object, class Transform *transform);
	~ParticleEmitter();

	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter &operator=(const ParticleEmitter&) = delete;

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

	VertexBuffer* GetVertexBuffer() const { return vertexBuffer; }
	IndexBuffer *GetIndexBuffer() const { return indexBuffer; }

private:
	void CreateParticleMesh();
	void UpdateParticleMesh();

	void EmitParticles(uint count);
	
	void CalcParticleLife(const uint &particleIndex);
	void CalcParticlePosition(const uint &particleIndex, const Vector2 &position);
	void CalcParticleSize(const uint &particleIndex);
	void CalcParticleColor(const uint &particleIndex);
	void CalcParticleRotation(const uint &particleIndex);
	void CalcParticleRadius(const uint &particleIndex);
private:
	class Timer *timer;
	Geometry<VertexTextureColor> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	ParticleData particleData;
	uint particleCount;
	float emitCounter; //방출간격
	float elapsed;
	float timeSinceUpdate;
	bool bActive;
	bool bRunning;
	bool bFinished;
	bool bNeedsMeshUpdate;

	std::vector<Particle> particles;
};