#pragma once
#include "ISubsystem.h"

class EffectManager : public ISubsystem
{
public:
	EffectManager(class Context* context);
	virtual ~EffectManager();

	void Initialize() override;

	void AddEffect
	(
		const std::string& textureFilePath,
		const std::string& animationFilePath,
		const Vector3& position,
		const Vector3& scale = Vector3(1.0f)
	);

	void Update();
	void Render();

private:
	std::vector<class Effect*> effects;
};