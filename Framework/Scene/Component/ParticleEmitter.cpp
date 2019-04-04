#include "Framework.h"
#include "ParticleEmitter.h"
#include "Transform.h"

static constexpr float UPDATE_STEP = 1.0f / 60.0f;

ParticleEmitter::ParticleEmitter(Context * context, GameObject * object, Transform * transform)
	: IComponent(context, object, transform), particleCount(0), emitCounter(0.0f), elapsed(0.0f), timeSinceUpdate(0.0f), bActive(false), bRunning(false), bFinished(false), bNeedsMeshUpdate(false)
{
	timer = context->GetSubsystem<Timer>();

	vertexBuffer = new VertexBuffer(context);
	indexBuffer = new IndexBuffer(context);
}

ParticleEmitter::~ParticleEmitter()
{
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void ParticleEmitter::OnInitialize()
{
}

void ParticleEmitter::OnStart()
{
	if (!bRunning) {
		bFinished = false;
		bRunning = true;

		if (!bActive) bActive = true;
	}
}

void ParticleEmitter::OnUpdate()
{

	if (bNeedsMeshUpdate) UpdateParticleMesh();

	timeSinceUpdate += timer->GetDeltaTimeSec();
	
	while (timeSinceUpdate >= UPDATE_STEP) {
		timeSinceUpdate -= UPDATE_STEP;

		if (bRunning &&particleData.emissionRate > 0.0f) { //실행중 & 방출개수 0개 위
			float rate = 1.0f / particleData.emissionRate;

			if (particleCount < particleData.maxParticles) {
				emitCounter += UPDATE_STEP;
				if (emitCounter < 0.0f) emitCounter = 0.0f;
			}

			//배출간격 / 간격으로 방출된 개수를 알아냄
			uint emitCount = static_cast<uint>(Math::Min(static_cast<float>(particleData.maxParticles - particleCount), emitCounter / rate));
			EmitParticles(emitCount);

			emitCounter -= rate * emitCount;

			elapsed += UPDATE_STEP;
			if (elapsed < 0.0f) elapsed = 0.0f;

			if (particleData.duration >= 0.0f && particleData.duration < elapsed) {
				bFinished = true;
				OnStop();
			}
		}

		if (bActive) {

		}
	}

}

void ParticleEmitter::OnStop()
{
}

void ParticleEmitter::OnDestroy()
{
}

void ParticleEmitter::CreateParticleMesh()
{
}

void ParticleEmitter::UpdateParticleMesh()
{
}

void ParticleEmitter::EmitParticles(uint count)
{
	if (particleCount + count > particleData.maxParticles) count = particleData.maxParticles - particleCount;

	if (count) {
		Vector2 position;

		if (particleData.positionType == PositionType::Free)
			position = Vector2(Vector3::TransformCoord(Vector3::Zero, transform->GetWorldMatrix()));
		else if (particleData.positionType == PositionType::Parent)
			position = Vector2(Vector3::TransformCoord(Vector3::Zero - transform->GetPosition(), transform->GetWorldMatrix()));

		for (uint i = particleCount; i < particleCount + count; i++) {
			if (particleData.emitterType == EmitterType::Gravity) {
				//Life
				particles[i].life = Math::Max(particleData.particleLifeSpan + particleData.particleLifeSpanVariance * Math::Random(-1.0f, -1.0f), 0.0f);
				
				//Position
				particles[i].position = particleData.sourcePosition + position
					+ Vector2(particleData.sourcePositionVariance.x * Math::Random(-1.0f, 1.0f), particleData.sourcePositionVariance.y * Math::Random(-1.0f, 1.0f));
			}
			else {

			}
		}

		particleCount += count;
	}
}
