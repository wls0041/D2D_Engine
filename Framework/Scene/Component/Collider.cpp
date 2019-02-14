#include "Framework.h"
#include "Collider.h"
#include "Transform.h"
#include "../GameObject.h"

Collider::Collider(Context * context, class GameObject *object, class Transform *transform)
	: IComponent(context, object, transform)
	, transform(nullptr)
	, Event(nullptr)
	, center(0.0f)
	, size(1.0f)
{
}

Collider::~Collider()
{
}

void Collider::OnInitialize()
{
}

void Collider::OnStart()
{
}

void Collider::OnUpdate()
{
}

void Collider::OnStop()
{
}

void Collider::OnDestroy()
{
}

const BoundBox Collider::GetBoundBox() const
{
	Vector3 minBox = Vector3
	(
		center - size * 0.5f
	);

	Vector3 maxBox = Vector3
	(
		center + size * 0.5f
	);

	return BoundBox(minBox, maxBox);
}

const bool Collider::AABB(Collider * collider)
{
	BoundBox boundBox = GetBoundBox();

	auto check = boundBox.IsInside(collider->GetBoundBox());
	return static_cast<uint>(check) > 0;
}
