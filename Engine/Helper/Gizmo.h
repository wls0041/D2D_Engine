#pragma once

//이미지를 출력하고 조작할 수 있게 만들어 주는 툴
class Gizmo final
{
public:
	static void TransformGizmo(class Camera *camera, class Transform *transform, const Vector2 &framePos, const Vector2 &frameSize);

};