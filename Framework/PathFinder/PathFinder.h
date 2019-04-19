#pragma once
#include "Framework.h"
#include "AstarNode.h"
#include "Astar.h"

class PathFinder final
{
public:
	PathFinder() : start(nullptr), goal(nullptr) {}
	~PathFinder() = default;

	auto GetStart() const -> AstarNode* { return start; }
	void SetStart(AstarNode *start) { this->start = start; }

	auto GetGoal() const -> AstarNode* { return goal; }
	void SetGoal(AstarNode *goal) { this->goal = goal; }

	auto FindPath(std::vector<AstarNode*> &solution) -> const bool {
		std::vector<AstarNode*> path;
		bool bPathFound = Astar::Get().GetPath(start, goal, path);

		if (!bPathFound) return false;
		for (auto riter = std::rbegin(path); riter != std::rend(path); riter++)	solution.push_back(*riter);
		
		return true;
	}

private:
	AstarNode *start;
	AstarNode *goal;
};
