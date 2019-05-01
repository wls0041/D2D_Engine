#pragma once
#include "Framework.h"

template <class TPathNode>
class IPathAlgorithm
{
public:
	typedef TPathNode PathNodeType;

public:
	virtual ~IPathAlgorithm() = default;

	virtual auto GetPath(TPathNode *start, TPathNode *goal, std::vector<TPathNode*> &path) -> const bool = 0;
	virtual void Clear() = 0;

	virtual auto GetDistanceBetween(TPathNode *lhs, TPathNode *rhs) -> const float { return lhs->GetDistance(rhs); }
	virtual void ReconstructPath(TPathNode *node, std::vector<TPathNode*> &path); {//되짚어 경로를 찾고 되짚어 온 길을 정방향으로 고침
		auto parent = node->GetParent();
		path.push_back(node);

		while (parent != nullptr) {
			path.push_back(parent);
			parent = parent->GetParent();
		}
	}
public:

};