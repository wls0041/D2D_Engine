#pragma once
#include "Framework.h"
#include "../PathNode/AstarNode.h"
#include "../PathAlgorithm/Astar.h"

template <class TPathNode>
class PathFinder final
{
public:
	PathFinder() : start(nullptr), goal(nullptr) {}
	~PathFinder() = default;

	auto GetStart() const -> TPathNode* { return start; }
	void SetStart(TPathNode *start) { this->start = start; }

	auto GetGoal() const -> TPathNode* { return goal; }
	void SetGoal(TPathNode *goal) { this->goal = goal; }

	template <typename TAlgorithm>
	auto FindPath(std::vector<TPathNode*> &solution) -> const bool {
		
		std::vector<typename TAlgorithm::PathNodeType*> path;
		bool bPathFound = TAlgorithm::Get().GetPath(start, goal, path);

		if (!bPathFound) return false;
		for (auto riter = std::rbegin(path); riter != std::rend(path); riter++)	solution.push_back(static_cast<TPathNode*>(*riter));
		
		return true;
	}

private:
	TPathNode *start;
	TPathNode *goal;
};
