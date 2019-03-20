#pragma once
#include "IComponent.h"

class Script final : public IComponent
{
public:
	Script(class Context *context, class GameObject *object, class Transform *transform);
	~Script();

	Script(const Script&) = delete;
	Script &operator=(const Script&) = delete;

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;
	
	auto GetScriptPath() -> const std::string;
	auto GetName() -> const std::string;

	auto SetScript(const std::string &path) -> const bool;

private:
	class ScriptInstance *scriptInstance; //script ��ü
	bool bStopped;
};