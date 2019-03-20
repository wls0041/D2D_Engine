#include "Framework.h"
#include "Script.h"
#include "../../Script/ScriptInstance.h"

Script::Script(Context * context, GameObject * object, Transform * transform) : IComponent(context, object, transform), scriptInstance(nullptr), bStopped(true)
{
}

Script::~Script()
{
	SAFE_DELETE(scriptInstance);
}

void Script::OnInitialize()
{
}

void Script::OnStart()
{
	if (!scriptInstance) return;
	if (!scriptInstance->IsIntantiated()) return;

	scriptInstance->ExecuteStart();
	bStopped = false;
}

void Script::OnUpdate()
{
	if (bStopped) return;
	if (!scriptInstance) return;
	if (!scriptInstance->IsIntantiated()) return;

	scriptInstance->ExecuteUpdate();
}

void Script::OnStop()
{
	bStopped = true;
}

void Script::OnDestroy()
{
}

auto Script::GetScriptPath() -> const std::string
{
	return scriptInstance ? scriptInstance->GetScriptPath() : "";
}

auto Script::GetName() -> const std::string
{
	return scriptInstance ? FileSystem::GetIntactFileNameFromFilePath(GetScriptPath()) : "";
}

auto Script::SetScript(const std::string & path) -> const bool
{
	SAFE_DELETE(scriptInstance);
	scriptInstance = new ScriptInstance();
	auto bResult = scriptInstance->Instantiate(path, object, context->GetSubsystem<Scripting>());
	
	return bResult;
}
