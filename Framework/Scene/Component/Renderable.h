#pragma once
#include "IComponent.h"

class Renderable final : public IComponent
{
public:
	Renderable(class Context* context, class GameObject *object, class Transform *transform);
	~Renderable();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

private:

};