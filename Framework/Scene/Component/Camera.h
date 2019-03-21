#pragma once
#include "IComponent.h"

enum class ProjectionType : uint { Perspective, Orthographic, };

class Camera final : public IComponent
{
public:
	Camera(class Context* context, class GameObject *object, class Transform *transform);
	Camera(class Context *context);
	virtual ~Camera();

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

	const Matrix& GetViewMatrix() const { return view; }
	const Matrix& GetProjectionMatrix() const { return projection; }
	auto GetProjectionType() const -> const ProjectionType& { return projectionType; }

	const float &GetNearPlane() const { return nearPlane; }
	const float &GetFarPlane() const { return farPlane; }
	const float &GetZoom() const { return zoom; }
	const bool &IsEditorCamera() const { return bEditorCamera; }

	void SetProjectionType(const ProjectionType &type) { this->projectionType = type; }
	void SetNearPlane(const float &nearPlane) { this->nearPlane = nearPlane; }
	void SetFarPlane(const float &farPlane) { this->farPlane = farPlane; }
	void SetZoom(const float &zoom) { this->zoom = zoom; }
	
	const Vector3 ScreenToWorldPoint(const Vector2& screenPoint);

	void UpdateEditorCamera();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	class Timer* timer;
	class Input* input;

	Matrix view;
	Matrix projection;

	ProjectionType projectionType;
	float nearPlane;
	float farPlane;
	float zoom;
	bool bEditorCamera; //editor�� �������� ������ ����. ����ī�޶�� ������ �������� ���󰡾���. bool�� editor�̸� �������� �̸� ����
};