#pragma once
#include "ISubsystem.h"

class ColliderManager : public ISubsystem
{
public:
	ColliderManager(class Context* context);
	virtual ~ColliderManager();

	void Initialize() override;

	void RegisterCollider
	(
		const std::string& layer,
		class Collider* collider
	);

	void RegisterColliders
	(
		const std::string& layer,
		const std::vector<class Collider*>& colliders
	);

	void Clear();

	void HitCheck_AABB
	(
		const std::string& attacker,
		const std::string& receiver
	);

private:
	std::map<std::string, std::vector<class Collider*>> colliderLayers;
};