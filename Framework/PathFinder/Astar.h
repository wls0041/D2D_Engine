#pragma once
#include "AstarNode.h"

class Astar final
{
public:
	static Astar &Get() {
		static Astar instance;
		return instance;
	}

	auto GetPath(AstarNode *start, AstarNode *goal, std::vector<AstarNode*> &path) -> const bool;

	void Clear();

private:
	Astar() = default;
	~Astar() = default;

	auto GetDistanceBetween(AstarNode *lhs, AstarNode *rhs) -> const float;
	void ReconstructPath(AstarNode *node, std::vector<AstarNode*> &path); //되짚어 경로를 찾고 되짚어 온 길을 정방향으로 고침
	void ReleaseNodes();

private:
	std::vector<AstarNode*> openList;
	std::vector<AstarNode*> closeList;
};