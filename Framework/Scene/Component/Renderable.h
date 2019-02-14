#pragma once
#include "IComponent.h"

class Renderable final : public IComponent
{
public:
	Renderable(class Context* context, class GameObject *object, class Transform *transform);
	~Renderable();

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

private:

};