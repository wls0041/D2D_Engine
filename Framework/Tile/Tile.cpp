#include "Framework.h"
#include "Tile.h"

Tile::Tile() : texturePath(""), offset(0.0f), size(1.0f), color(1.0f)
{
}

auto Tile::GetPosition() const -> const Vector3
{
	return Vector3(x, y, 0.0f);
}

void Tile::SetPosition(const Vector2 & position)
{
	x = position.x;
	y = position.y;
}

const float Tile::GetLocalDistance(AstarNode * node)
{
	if (node->GetX() != x && node->GetY() != y) return 1.41421356237f;
	else return 1.0f;
}

const float Tile::GetDistance(AstarNode * node)
{
	//Euclidean Distance - L = sqrt(|x1 - x2|^2, |y1 - y2|^2);  정확. 계산느림
	///float newX = x - node->GetX();
	///float newY = y - node->GetY();
	///return sqrt(pow(newX, 2) + pow(newY, 2));
	

	//Manhattan Distance - L = |x1 - x2| + |y1 - y2|; 부정확. 계산 빠름
	float newX = Math::Abs(x - node->GetX());
	float newY = Math::Abs(y - node->GetY());

	return newX + newY;
}