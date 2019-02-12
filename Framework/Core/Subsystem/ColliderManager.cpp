#include "Framework.h"
#include "ColliderManager.h"
#include "../../Scene/Component/Collider.h"

ColliderManager::ColliderManager(Context * context)
	: ISubsystem(context)
{
}

ColliderManager::~ColliderManager()
{
	Clear();
}

void ColliderManager::Initialize()
{
}

void ColliderManager::RegisterCollider(const std::string & layer, Collider * collider)
{
	colliderLayers[layer].emplace_back(collider);
}

void ColliderManager::RegisterColliders(const std::string & layer, const std::vector<class Collider*>& colliders)
{
	colliderLayers[layer].insert
	(
		colliderLayers[layer].end(),
		colliders.begin(),
		colliders.end()
	);
}

void ColliderManager::Clear()
{
	for (auto& layer : colliderLayers)
	{
		layer.second.clear();
		layer.second.shrink_to_fit();
	}

	colliderLayers.clear();
}

void ColliderManager::HitCheck_AABB(const std::string & attacker, const std::string & receiver)
{
	for(auto receiverCollider : colliderLayers[receiver])
		for (auto attackerCollider : colliderLayers[attacker])
		{
			bool bCheck = receiverCollider->AABB(attackerCollider);

			if (bCheck)
			{
				if (receiverCollider->Event != nullptr)
					receiverCollider->Event();
			}
		}
}
