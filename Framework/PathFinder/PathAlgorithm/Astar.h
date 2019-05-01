#pragma once
#include "IPathAlgorithm.h"
#include "../PathNode/AstarNode.h"

class Astar final : public IPathAlgorithm<AstarNode>
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

	void ReleaseNodes();

private:
	std::vector<AstarNode*> openList;
	std::vector<AstarNode*> closeList;
};