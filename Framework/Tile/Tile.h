#pragma once
#include "../PathFinder/PathNode/AstarNode.h"

class Tile final : public AstarNode
{
public:
	Tile();
	~Tile() = default;

	Tile(const Tile&) = delete;
	Tile &operator=(const Tile&) = delete;

	auto GetOffset() const -> const Vector2& { return offset; }
	void SetOffset(const Vector2 &offset) { this->offset = offset; }

	auto GetTilesetSize() const -> const Vector2& { return tilesetSize; }
	void SetTilesetSize(const Vector2 &size) { this->tilesetSize = size; }

	auto GetSize() const -> const Vector2& { return size; }
	void SetSize(const Vector2 &size) { this->size = size; }

	auto GetScale() const -> const Vector3;
	void SetScale(const Vector2 &scale);
	void SetScale(const float &scale);

	auto GetPosition() const -> const Vector3;
	void SetPosition(const Vector2 &position);

	auto GetColor() const -> const Color& { return color; }
	void SetColor(const Color &color) { this->color = color; if (color == Color::Red) bWalkable = false; }

	auto GetTilesetIndex() const -> const int& { return tilesetIndex; }
	void SetTilesetIndex(const int &index) { this->tilesetIndex = index; }

	const bool &IsWalkable() { return bWalkable; }
	const bool IsWalkable(const bool &bWalkable) { this->bWalkable = bWalkable; }

	const float GetLocalDistance(AstarNode *node);
	const float GetDistance(AstarNode *node) override;

private:
	Vector2 offset;
	Vector2 size; //sprite
	Vector2 tilesetSize;
	Vector2 scale;

	Color color;
	int tilesetIndex;
	bool bWalkable;
};
