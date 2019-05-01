#pragma once
#include "Framework.h"

class IPathNode
{
public:
	IPathNode();
	virtual ~IPathNode() = default;
	
	auto GetParent() const ->IPathNode* { return parent; }
	void SetParent(IPathNode *parent) { this->parent = parent; }

	auto GetChilds() const -> const std::vector<std::pair<IPathNode*, float>>& { return childs; }
	void AddChild(IPathNode *child, const float &distance) { childs.emplace_back(child, distance); }

protected:
	void ClearChilds()
	{
		childs.clear();
		childs.shrink_to_fit();
	}

protected:
	IPathNode *parent;
	std::vector<std::pair<IPathNode*, float>> childs;
};