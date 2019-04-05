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
	particleData.startColorA = 0.5;
	particleData.startParticleSizeVariance = 5;
	particleData.startColorG = 0.4040371f;
	particleData.rotationPerSecond = 360;
	particleData.radialAccelation = 0;
	particleData.bYCoordFlipped = 1;
	particleData.positionType = PositionType::Free;
	particleData.emitterType = EmitterType::Gravity;
	particleData.blendFuncSource = 1;
	particleData.finishColorAVariance = 0;
	particleData.finishRotation = 0;
	particleData.startColorBVariance = 0;
	particleData.rotationPerSecondVariance = 0;
	particleData.particleLifeSpan = 1;
	particleData.minRadius = 300;
	particleData.tangentialAccelationVariance = 0;
	particleData.startRotation = 0;
	particleData.startColorGVariance = 0;
	particleData.speed = 225;
	particleData.minRadiusVariance = 0;
	particleData.finishColorBVariance = 0;
	particleData.finishColorB = 0;
	particleData.finishColorG = 0;
	particleData.blendFuncDestination = 771;
	particleData.finishColorA = 0;
	particleData.sourcePositionVariance = Vector2(7, 7);
	particleData.startParticleSize = 64;
	particleData.startColorR = 0.8448572f;
	particleData.finishColorRVariance = 0;
	particleData.bAbsolutePosition = false;
	particleData.startColorAVariance = 0;
	particleData.maxParticles = 77;
	particleData.finishColorGVariance = 0;
	particleData.finishParticleSize = 0;
	particleData.duration = -1;
	particleData.startColorRVariance = 0;
	particleData.finishColorR = 0;
	particleData.gravity = Vector2(0, 0);
	particleData.maxRadiusVariance = 0;
	particleData.finishParticleSizeVariance = 0;
	particleData.finishRotationVariance = 0;
	particleData.startColorBVariance = 0;
	particleData.startRotationVariance = 0;
	particleData.speedVariance = 30;
	particleData.radialAccelationVariance = 0;
	particleData.tangentialAccelationVariance = 0;
	particleData.particleLifeSpanVariance = 0;
	particleData.angleVariance = 10;
	particleData.angle = 90;
	particleData.maxRadius = 0;
	particleData.emissionRate = 100.0f;

	CreateParticleMesh();
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
			for (int counter = static_cast<int>(particleCount); counter > 0; counter--) {
				uint i = counter - 1;

				particles[i].life -= UPDATE_STEP;
				if (particles[i].life == 0.0f) {
					if (particleData.emitterType == EmitterType::Gravity) {
						Vector2 temp, radial, tangential;

						//Radial Accelation
						if (particles[i].position.x == 0.0f || particles[i].position.y == 0.0f) {
							radial = particles[i].position;
							radial.Normalize();
						}

						tangential = radial;
						radial *= particles[i].radialAccelation;

						//Tangential Accelation
						std::swap(tangential.x, tangential.y);
						tangential.x *= -particles[i].tangentialAccelation;
						tangential.y *= particles[i].tangentialAccelation;

						//(gravity + radial + tangential) * update_step
						temp.x = radial + tangential + particleData.gravity.x;
						temp.y = radial + tangential + particleData.gravity.y;
						temp.x *= UPDATE_STEP;
						temp.y *= UPDATE_STEP;

						particles[i].direction.x += temp.x;
						particles[i].direction.y += temp.y;

						temp.x = particles[i].direction.x *UPDATE_STEP * particleData.bYCoordFlipped;
						temp.y = particles[i].direction.x *UPDATE_STEP * particleData.bYCoordFlipped;

						particles[i].position.x += temp.x;
						particles[i].position.y += temp.y;
					}
					else {
						particles[i].angle += particles[i].degreePerSecond * UPDATE_STEP;
						particles[i].radius += particles[i].deltaRadius * UPDATE_STEP;
						particles[i].position.x = -cosf(particles[i].angle) * particles[i].radius;
						particles[i].position.y = -sinf(particles[i].angle) * particles[i].radius * particleData.bYCoordFlipped;

						//Color RGBA
						particles[i].colorR += particles[i].deltaColorR * UPDATE_STEP;
						particles[i].colorG += particles[i].deltaColorG * UPDATE_STEP;
						particles[i].colorB += particles[i].deltaColorB * UPDATE_STEP;
						particles[i].colorA += particles[i].deltaColorA * UPDATE_STEP;

						//Size
						particles[i].size += particles[i].deltaSize * UPDATE_STEP;
						particles[i].size = Math::Max(0.0f, particles[i].size);

						//Angle
						particles[i].rotation += particles[i].deltaRotation * UPDATE_STEP;
					}
				}
				else {
					particles[i] = particles[particleCount - 1];
					particleCount--;
				}
			}
			bNeedsMeshUpdate = true;
		}
	}

}

void ParticleEmitter::OnStop()
{
	bRunning = false;
}

void ParticleEmitter::OnDestroy()
{
}

void ParticleEmitter::CreateParticleMesh()
{
	particles.clear();
	particles.shrink_to_fit();

	geometry.Clear();
	vertexBuffer->Clear();
	indexBuffer->Clear();

	for (uint i = 0; i < particleData.maxParticles; i++) {
		geometry.AddVertex({ Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f), Color::White });
		geometry.AddVertex({ Vector3(-1.0f, +1.0f, 0.0f), Vector2(0.0f, 0.0f), Color::White });
		geometry.AddVertex({ Vector3(+1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f), Color::White });
		geometry.AddVertex({ Vector3(+1.0f, +1.0f, 0.0f), Vector2(1.0f, 0.0f), Color::White });

		geometry.AddIndex(i * 4 + 0);
		geometry.AddIndex(i * 4 + 1);
		geometry.AddIndex(i * 4 + 2);
		geometry.AddIndex(i * 4 + 2);
		geometry.AddIndex(i * 4 + 1);
		geometry.AddIndex(i * 4 + 3);
	}

	vertexBuffer->Create(geometry.GetVertices(), D3D11_USAGE_DYNAMIC); //정점자체가 돌아다니면서 gpu로의 업데이트가 필요해 질 수 있기 때문에 dynamic사용
	indexBuffer->Create(geometry.GetIndices(), D3D11_USAGE_DYNAMIC);

	particles.resize(particleData.maxParticles);
}

void ParticleEmitter::UpdateParticleMesh()
{
	auto vertices = geometry.GetVertexData(); //vector의 정보를 꺼내는 at은 속도가 매우 느려 포인터로 꺼내 씀

	for (uint counter = static_cast<int>(particleCount); counter > 0; counter--) {
		uint i = counter - 1;
		Vector2 position;

		if (particleData.positionType == PositionType::Free) position = particles[i].position;
		else if (particleData.positionType == PositionType::Parent) position = Vector2(transform->GetPosition()) + particles[i].position;
		
		float halfSize = particles[i].size * 0.5f;
		Vector2 v1 = Vector2(-halfSize, -halfSize);
		Vector2 v2 = Vector2(+halfSize, +halfSize);

		float rot = -Math::ToRadian(particles[i].rotation);
		float cRot = cosf(rot);
		float sRot = sinf(rot);

		Vector2 e0 = Vector2(v1.x * cRot - v1.y * sRot, v1.x *sRot + v1.y * cRot);
		Vector2 e1 = Vector2(v1.x * cRot - v2.y * sRot, v1.x *sRot + v2.y * cRot);
		Vector2 e2 = Vector2(v2.x * cRot - v1.y * sRot, v2.x *sRot + v1.y * cRot);
		Vector2 e3 = Vector2(v2.x * cRot - v2.y * sRot, v2.x *sRot + v2.y * cRot);

		Color color = Color(particles[i].colorR, particles[i].colorG, particles[i].colorB, particles[i].colorA);

		vertices[i * 4 + 0].position = Vector3(e0 + position);
		vertices[i * 4 + 0].color = color;

		vertices[i * 4 + 1].position = Vector3(e1 + position);
		vertices[i * 4 + 1].color = color;
		
		vertices[i * 4 + 2].position = Vector3(e2 + position);
		vertices[i * 4 + 2].color = color;
		
		vertices[i * 4 + 3].position = Vector3(e3 + position);
		vertices[i * 4 + 3].color = color;
	}

	auto data = vertexBuffer->Map();
	memcpy(data, vertices, geometry.GetVertexDataSize());
	vertexBuffer->Unmap();
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
				CalcParticleLife(i);
				CalcParticlePosition(i, position);
				CalcParticleSize(i);
				CalcParticleColor(i);
				CalcParticleRotation(i);
			}
			else {
				CalcParticleRadius(i);
			}
		}

		particleCount += count;
	}
}

void ParticleEmitter::CalcParticleLife(const uint & particleIndex)
{
	//Particle &particle = particles[particleIndex];
	//particle.life  이렇게 해도 됨

	particles[particleIndex].life = Math::Max(particleData.particleLifeSpan + particleData.particleLifeSpanVariance * Math::Random(-1.0f, -1.0f), 0.0f);

}

void ParticleEmitter::CalcParticlePosition(const uint & particleIndex, const Vector2 &position)
{
	//Position
	particles[particleIndex].position = particleData.sourcePosition + position
		+ Vector2(particleData.sourcePositionVariance.x * Math::Random(-1.0f, 1.0f), particleData.sourcePositionVariance.y * Math::Random(-1.0f, 1.0f));

}

void ParticleEmitter::CalcParticleSize(const uint & particleIndex)
{
	//Size
	particles[particleIndex].size = Math::Max(particleData.startParticleSize + particleData.startParticleSizeVariance * Math::Random(-1.0f, 1.0f), 0.0f);
	float finishSize = Math::Max(particleData.finishParticleSizeVariance * Math::Random(-1.0f, 1.0f), 0.0f);
	particles[particleIndex].deltaSize = (finishSize - particles[particleIndex].size) / particles[particleIndex].life;

}

void ParticleEmitter::CalcParticleColor(const uint & particleIndex)
{
	//Color
	particles[particleIndex].colorR = Math::Clamp(particleData.startColorR + particleData.startColorRVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	particles[particleIndex].colorG = Math::Clamp(particleData.startColorG + particleData.startColorGVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	particles[particleIndex].colorB = Math::Clamp(particleData.startColorB + particleData.startColorBVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	particles[particleIndex].colorA = Math::Clamp(particleData.startColorA + particleData.startColorAVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);

	float finishColorR = Math::Clamp(particleData.finishColorR + particleData.finishColorRVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	float finishColorG = Math::Clamp(particleData.finishColorG + particleData.finishColorGVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	float finishColorB = Math::Clamp(particleData.finishColorB + particleData.finishColorBVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);
	float finishColorA = Math::Clamp(particleData.finishColorA + particleData.finishColorAVariance * Math::Random(-1.0f, 1.0f), 0.0f, 1.0f);

	particles[particleIndex].deltaColorR = (finishColorR - particles[particleIndex].colorR) / particles[particleIndex].life;
	particles[particleIndex].deltaColorG = (finishColorG - particles[particleIndex].colorG) / particles[particleIndex].life;
	particles[particleIndex].deltaColorB = (finishColorB - particles[particleIndex].colorB) / particles[particleIndex].life;
	particles[particleIndex].deltaColorA = (finishColorA - particles[particleIndex].colorA) / particles[particleIndex].life;
}

void ParticleEmitter::CalcParticleRotation(const uint & particleIndex)
{
	Particle &particle = particles[particleIndex];

	particle.rotation = particleData.startRotation + particleData.startRotationVariance * Math::Random(-1.0f, 1.0f);
	float finishRotation = particleData.finishRotation + particleData.finishRotationVariance * Math::Random(-1.0f, 1.0f);
	particle.deltaRotation = (finishRotation - particle.rotation) / particle.life;

	particle.radialAccelation = particleData.radialAccelation + particleData.radialAccelationVariance * Math::Random(-1.0f, 1.0f);
	particle.tangentialAccelation = particleData.tangentialAccelation + particleData.tangentialAccelationVariance * Math::Random(-1.0f, 1.0f);

	
	float angle = Math::ToRadian(particleData.angle + particleData.angleVariance * Math::Random(-1.0f, 1.0f));
	Vector2 velocity = Vector2(cosf(angle), sinf(angle));
	float speed = particleData.speed + particleData.speedVariance * Math::Random(-1.0f, 1.0f);

	Vector2 direction = velocity * speed;
	particle.direction = direction;
	
	if (particleData.bRotationIsDirection) particle.rotation = -Math::ToDegree(atan2f(direction.y, direction.x)); //방향에 영향을 받는 경우 각을 계산
}

void ParticleEmitter::CalcParticleRadius(const uint & particleIndex)
{
	Particle &particle = particles[particleIndex];
	
	particle.radius = particleData.maxRadius + particleData.maxRadiusVariance * Math::Random(-1.0f, 1.0f);
	particle.angle = Math::ToRadian(particleData.angle + particleData.angleVariance * Math::Random(-1.0f, 1.0f));
	particle.degreePerSecond = Math::ToRadian(particleData.rotationPerSecond + particleData.rotationPerSecondVariance * Math::Random(-1.0f, 1.0f));

	float finishRadius = particleData.minRadius + particleData.minRadiusVariance * Math::Random(-1.0f, 1.0f);

	particle.deltaRadius = (finishRadius - particle.radius) / particle.life;

}

