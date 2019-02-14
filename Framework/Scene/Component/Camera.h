#pragma once
#include "IComponent.h"

class Camera final : public IComponent
{
public:
	Camera(class Context* context, class GameObject *object, class Transform *transform);
	virtual ~Camera();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

	const Matrix& GetViewMatrix() const { return view; }
	const Matrix& GetProjectionMatrix() const { return projection; }
	const Vector3& GetPosition() const { return position; }

	const Vector3 ScreenToWorldPoint(const Vector2& screenPoint);

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	class Context* context;
	class Timer* timer;
	class Input* input;

	Vector3 position;
	Matrix view;
	Matrix projection;

	float zoom;
};