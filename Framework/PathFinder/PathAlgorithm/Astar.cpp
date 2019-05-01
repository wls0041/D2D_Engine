#include "Framework.h"
#include "Astar.h"
#include <algorithm>

auto Astar::GetPath(AstarNode * start, AstarNode * goal, std::vector<AstarNode*>& path) -> const bool
{
	AstarNode *currentNode = nullptr, *childNode = nullptr;
	float f = 0, g = 0, h = 0; //; == sequence point

	openList.push_back(start); //시작점
	start->SetIsOpen(true);

	while (!openList.empty()) {
		//priority queue
		std::sort(openList.begin(), openList.end(), CompareGreaterAstarNode());  //sort. 기본값은 오름차순, greater~ 함수객체
		
		currentNode = openList.back();
		openList.pop_back();
		currentNode->SetIsOpen(false);

		closeList.push_back(currentNode);
		currentNode->SetIsClose(true);

		if (currentNode == goal) {
			ReconstructPath(currentNode, path);
			return true;
		}

		for (const auto &child : currentNode->GetChilds()) {
			childNode = child.first;

			g = currentNode->GetG() + child.second;
			
			bool bCheck = true;
			bCheck &= childNode->IsOpen() || childNode->IsClose();
			bCheck &= childNode->GetG() > g;

			if (bCheck) continue;

			h = GetDistanceBetween(childNode, goal);
			f = g + h;

			childNode->SetF(f);
			childNode->SetG(g);
			childNode->SetH(h);
			childNode->SetParent(currentNode);

			if (childNode->IsClose()) childNode->SetIsClose(false);
			if (!childNode->IsOpen()) {
				openList.push_back(childNode);
				childNode->SetIsOpen(true);
			}
		}
	}

	return false;
}

void Astar::Clear()
{
	ReleaseNodes();

	openList.clear();
	openList.shrink_to_fit();

	closeList.clear();
	closeList.shrink_to_fit();
}

void Astar::ReleaseNodes()
{
	for (const auto &node : openList) node->Release();
	for (const auto &node : closeList) node->Release();
}
