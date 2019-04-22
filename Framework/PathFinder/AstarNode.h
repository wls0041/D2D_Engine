#pragma once
#include "Framework.h"

//길찾기. 최단거리
class AstarNode
{
public:
	AstarNode() : f(0.0f), g(0.0f), h(0.0f), x(0.0f), y(0.0f), bOpen(false), bClose(false) {}
	virtual ~AstarNode() = default;

	auto GetParent() const -> AstarNode* { return parent; }
	void SetParent(AstarNode *parent) { this->parent = parent; }

	auto Getchilds() const -> const std::vector<std::pair<AstarNode*, float>>& { return childs; }
	void AddChild(AstarNode *child, const float &distance) { childs.emplace_back(child, distance); }

	auto GetX() const -> const float& { return x; }
	void SetX(const float &x) { this->x = x; }

	auto GetY() const -> const float& { return y; }
	void SetY(const float &y) { this->y = y; }

	auto GetF() const -> const float& { return f; }
	void SetF(const float &f) { this->f = f; }

	auto GetG() const -> const float& { return g; }
	void SetG(const float &g) { this->g = g; }

	auto GetH() const -> const float& { return h; }
	void SetH(const float &h) { this->h = h; }

	auto IsOpen() const -> const bool& { return bOpen; }
	void SetIsOpen(const bool &bOpen) { this->bOpen = bOpen; }

	auto IsClose() const -> const bool& { return bClose; }
	void SetIsClose(const bool &bClose) { this->bClose = bClose; }

	virtual const float GetDistance(AstarNode *node) = 0;

	void Release() {
		bOpen = bClose = false;
		f = g = h = x = y = 0.0f;
		parent = nullptr;
	}

protected:
	//f = g + h
	float f; //비용
	float g; //시작점부터 새로운 지점까지의 이동비용 - 현재 상태(다음 단계)의 비용
	float h; //최종 목적지 까지의 예상 비용 - Heuristics
	float x;
	float y;
	
	bool bOpen;
	bool bClose;

	AstarNode *parent;
	std::vector<std::pair<AstarNode*, float>> childs;
};

struct CompareLessAstarNode { //오름차순. 작은 -> 큰
	bool operator() (const AstarNode *lhs, const AstarNode *rhs) const {
		return lhs->GetF() < rhs->GetF();
	}
};

struct CompareGreaterAstarNode { //내림차순. 큰 -> 작은
	bool operator() (const AstarNode *lhs, const AstarNode *rhs) const {
		return lhs->GetF() > rhs->GetF();
	}
};