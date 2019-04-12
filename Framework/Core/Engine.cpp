#include "Framework.h"
#include "Engine.h"

unsigned long Engine::engineFlags = 0UL;

Engine::Engine()
{
	engineFlags =
		EngineFlags_Update |
		EngineFlags_Render;

	context = new Context();

	context->RegisterSubsystem<Timer>();
	context->RegisterSubsystem<Input>();
	//context->RegisterSubsystem<Audio>();
	context->RegisterSubsystem<Thread>();
	context->RegisterSubsystem<Graphics>();
	context->RegisterSubsystem<ResourceManager>();
	context->RegisterSubsystem<ColliderManager>();
	context->RegisterSubsystem<Scripting>();
	context->RegisterSubsystem<Renderer>();
	context->RegisterSubsystem<SceneManager>();

	context->InitializeSubsystems();
}

Engine::~Engine()
{
	SAFE_DELETE(context);
}

void Engine::Update()
{
	if (IsOnEngineFlags(EngineFlags_Update))
		EventSystem::Get().Fire(EventType::Event_Update);

	if (IsOnEngineFlags(EngineFlags_Render))
		EventSystem::Get().Fire(EventType::Event_Render);
}
