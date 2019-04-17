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
	void ReconstructPath(AstarNode *node, std::vector<AstarNode*> &path);
	void ReleaseNodes();

private:
	std::vector<AstarNode*> openList;
	std::vector<AstarNode*> closeList;
};