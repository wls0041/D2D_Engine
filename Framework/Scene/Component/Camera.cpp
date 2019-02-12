#include "Framework.h"
#include "Camera.h"

Camera::Camera(Context * context)
	: context(context)
	, position(0, 0, 0)
	, zoom(1.0f)
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();

	view.SetIdentity();
	projection.SetIdentity();
}

Camera::~Camera()
{
}

const Vector3 Camera::ScreenToWorldPoint(const Vector2 & screenPoint)
{
	auto viewport = Settings::Get().GetViewPort();

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

void Camera::Update()
{
	if (input->KeyPress(VK_SHIFT))
	{
		if (input->KeyPress('W'))
			position += Vector3::Up * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('S'))
			position -= Vector3::Up * 200.0f * timer->GetDeltaTimeSec();

		if (input->KeyPress('A'))
			position -= Vector3::Right * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('D'))
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

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	view = Matrix::LookAtLH
	(
		position,
		(position + Vector3::Forward),
		Vector3::Up
	);
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = Settings::Get().GetViewPort();

	projection = Matrix::OrthoLH
	(
		viewport.Width / zoom,
		viewport.Height / zoom,
		0.0f,
		1.0f
	);
}
