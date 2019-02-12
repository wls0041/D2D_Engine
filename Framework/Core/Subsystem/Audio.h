#pragma once
#include "ISubsystem.h"

class Audio : public ISubsystem
{
public:
	Audio(class Context* context);
	virtual ~Audio();
	
	void Initialize() override;
	void Update();

	FMOD::System* GetFMODSystem() const { return system; }

private:
	FMOD::System* system;
	int maxChannel;
	float distanceFactor;
	bool bInitialized;

	int check;
};