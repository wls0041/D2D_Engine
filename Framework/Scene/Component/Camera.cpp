#include "Framework.h"
#include "Camera.h"
#include "Transform.h"
#include "../GameObject.h"

Camera::Camera(Context * context, GameObject * object, Transform * transform)
	: IComponent(context, object, transform)
	, timer(nullptr)
	, input(nullptr)
	, projectionType(ProjectionType::Orthographic)
	, nearPlane(0.0f)
	, farPlane(1.0f)
	, zoom(1.0f)
	, bEditorCamera(false)
{
	view.SetIdentity();
	projection.SetIdentity();
}

Camera::Camera(Context * context)
	: IComponent(context, nullptr, nullptr)
	, projectionType(ProjectionType::Orthographic)
	, nearPlane(0.0f)
	, farPlane(1.0f)
	, zoom(1.0f)
	, bEditorCamera(true)
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();
	transform = new Transform(context, nullptr, nullptr);

	view.SetIdentity();
	projection.SetIdentity();
}

Camera::~Camera()
{
	if (bEditorCamera)
		SAFE_DELETE(transform);
}

void Camera::OnInitialize()
{
}

void Camera::OnStart()
{
}

void Camera::OnUpdate()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::OnStop()
{
}

void Camera::OnDestroy()
{
}

const Vector3 Camera::ScreenToWorldPoint(const Vector2 & screenPoint)
{
	auto viewport = context->GetSubsystem<Renderer>()->GetViewport();

	//화면 좌표를 view 공간으로 이동
	float pointX = +2.0f * screenPoint.x / viewport.Width - 1.0f;
	float pointY = -2.0f * screenPoint.y / viewport.Height + 1.0f;

	//Unprojection
	Matrix unprojection = (view * projection).Inverse();

	return Vector3::TransformCoord
	(
		Vector3(pointX, pointY, 0.0f),
		unprojection
	);
}

void Camera::UpdateEditorCamera()
{
	if (!bEditorCamera)
		return;

	Vector3 position = transform->GetPosition();

	if (input->KeyPress(KeyCode::KEY_SHIFT))
	{
		if (input->KeyPress(KeyCode::KEY_W))
			position += Vector3::Up * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress(KeyCode::KEY_S))
			position -= Vector3::Up * 200.0f * timer->GetDeltaTimeSec();

		if (input->KeyPress(KeyCode::KEY_A))
			position -= Vector3::Right * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress(KeyCode::KEY_D))
			position += Vector3::Right * 200.0f * timer->GetDeltaTimeSec();

		float delta = input->GetMouseMoveValue().z;
		if (Math::Abs(delta) >= WHEEL_DELTA)
		{
			float deltaZoom = 1.0f - Math::Abs(delta) / WHEEL_DELTA / 10.0f;

			if (delta < 0)
				zoom *= deltaZoom;
			else
				zoom /= deltaZoom;

			zoom = Math::Clamp(zoom, 0.1f, 100.0f);
		}
	}

	transform->SetPosition(position);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	Vector3 position = transform->GetPosition();

	view = Matrix::LookAtLH
	(
		position,
		(position + Vector3::Forward),
		Vector3::Up
	);
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = context->GetSubsystem<Renderer>()->GetViewport();

	switch (projectionType)
	{
	case ProjectionType::Perspective:
		LOG_ERROR("Camera::UpdateProjectionMatrix : Not Supported Perspective Camera");
		break;
	case ProjectionType::Orthographic:
		projection = Matrix::OrthoLH
		(
			viewport.Width / zoom,
			viewport.Height / zoom,
			nearPlane,
			farPlane
		);
		break;
	default:
		break;
	}

}
