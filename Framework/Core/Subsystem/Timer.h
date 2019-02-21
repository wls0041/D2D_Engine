#pragma once
#include "ISubsystem.h"

class Timer : public ISubsystem
{
public:
	Timer(class Context* context);
	virtual ~Timer();

	const bool Initialize() override;
	void Update();

	const float& GetDeltaTimeMs() const { return deltaTimeMs; }
	const float& GetDeltaTimeSec() const { return deltaTimeSec; }

private:
	std::chrono::high_resolution_clock::time_point prevTime;

	float deltaTimeMs;
	float deltaTimeSec;
};