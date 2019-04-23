#pragma once
#include "../PathFinder/AstarNode.h"

class Tile final : public AstarNode
{
public:
	Tile();
	~Tile() = default;

	Tile(const Tile&) = delete;
	Tile &operator=(const Tile&) = delete;

	auto GetOffset() const -> const Vector2& { return offset; }
	void SetOffset(const Vector2 &offset) { this->offset = offset; }

	auto GetSize() const -> const Vector2& { return size; }
	void SetSize(const Vector2 &size) { this->size = size; }

	auto GetScale() const -> const Vector3;
	void SetScale(const Vector2 &scale);
	void SetScale(const float &scale);

	auto GetPosition() const -> const Vector3;
	void SetPosition(const Vector2 &position);

	auto GetColor() const -> const Color& { return color; }
	void SetColor(const Color &color) { this->color = color; }

	const float GetLocalDistance(AstarNode *node);
	const float GetDistance(AstarNode *node) override;
	
private:
	Vector2 offset;
	Vector2 size; //sprite
	Vector2 scale;
	Color color;
	uint tilesetIndex;
};
