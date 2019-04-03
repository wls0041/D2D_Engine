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

private:
	void CreateParticleMesh();
	void UpdateParticleMesh();

	void EmitParticles(const uint &count);

private:
	class Timer *timer;

	ParticleData particleData;
	uint particleCout;
	float emitCounter;
	float elapsed;
	float timeSinceUpdate;
	bool bActive;
	bool bRunning;
	bool bFinished;
	bool bNeedsMeshUpdate;

	std::vector<Particle> particles;
};