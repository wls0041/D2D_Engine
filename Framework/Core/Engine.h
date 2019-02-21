#pragma once

enum EngineFlags : unsigned long
{
	EngineFlags_Update	= 1UL << 0,
	EngineFlags_Render	= 1UL << 1,
	EngineFlags_Game	= 1UL << 2,
};

class Engine final
{
public:
	static const unsigned long& GetEngineFlags() { return engineFlags; }
	static void SetEngineFlags(const unsigned long& flags) { engineFlags = flags; }
	static void OnEngineFlags(const EngineFlags& flag) { engineFlags |= flag; }
	static void OffEngineFlags(const EngineFlags& flag) { engineFlags &= ~flag; }
	static void ToggleEngineFlags(const EngineFlags& flag) { IsOnEngineFlags(flag) ? OffEngineFlags(flag) : OnEngineFlags(flag); }
	static const bool IsOnEngineFlags(const EngineFlags& flag) { return (engineFlags & flag) > 0UL; }

public:
	Engine();
	~Engine();

	class Context* GetContext() const { return context; }

	void Update();

private:
	static unsigned long engineFlags;

	class Context*context;
	class Timer* timer;
	class Input* input;
	class SceneManager* sceneManager;
};