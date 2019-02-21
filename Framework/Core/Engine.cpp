#include "Framework.h"
#include "Engine.h"

unsigned long Engine::engineFlags = 0UL;

Engine::Engine()
	: timer(nullptr)
	, input(nullptr)
	, sceneManager(nullptr)
{
	engineFlags =
		EngineFlags_Update |
		EngineFlags_Render;

	context = new Context();

	context->RegisterSubsystem<Timer>();
	context->RegisterSubsystem<Input>();
	context->RegisterSubsystem<Audio>();
	context->RegisterSubsystem<Graphics>();
	context->RegisterSubsystem<DirectWrite>();
	context->RegisterSubsystem<ResourceManager>();
	context->RegisterSubsystem<ColliderManager>();
	context->RegisterSubsystem<SceneManager>();
	context->RegisterSubsystem<EffectManager>();

	context->InitializeSubsystems();
}

Engine::~Engine()
{
	SAFE_DELETE(context);
}

void Engine::Update()
{
}
