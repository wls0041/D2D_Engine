#pragma once
#include "IComponent.h"
#include "../../Particle/Particle.h"
#include "../../Particle/ParticleData.h"

class ParticleEmitter final : IComponent
{
public:
	ParticleEmitter(class Context *context, class GameObject *object, class Transform *transform);
	~ParticleEmitter();

	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter &operator=(const ParticleEmitter&) = delete;

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

	VertexBuffer* GetVertexBuffer() const { return vertexBuffer; }
	IndexBuffer *GetIndexBuffer() const { return indexBuffer; }

private:
	void CreateParticleMesh();
	void UpdateParticleMesh();

	void EmitParticles(uint count);

private:
	class Timer *timer;
	Geometry<VertexTextureColor> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	ParticleData particleData;
	uint particleCount;
	float emitCounter; //���Ⱓ��
	float elapsed;
	float timeSinceUpdate;
	bool bActive;
	bool bRunning;
	bool bFinished;
	bool bNeedsMeshUpdate;

	std::vector<Particle> particles;
};