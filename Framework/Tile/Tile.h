#pragma once
#include "../PathFinder/AstarNode.h"

class Tile final : public AstarNode
{
public:
	Tile();
	~Tile() = default;

	Tile(const Tile&) = delete;
	Tile &operator=(const Tile&) = delete;

	auto GetTexturePath() const -> const std::string& { return texturePath; }
	void SetTexturePath(const std::string &path) { this->texturePath = path; }

	auto GetOffset() const -> const Vector2& { return offset; }
	void SetOffset(const Vector2 &offset) { this->offset = offset; }

	auto GetSize() const -> const Vector2& { return size; }
	void SetSize(const Vector2 &size) { this->size = size; }

	auto GetPosition() const -> const Vector3;
	void SetPosition(const Vector2 &position);

	auto GetColor() const -> const Color& { return color; }
	void SetColor(const Color &color) { this->color = color; }

	const float GetLocalDistance(AstarNode *node);
	const float GetDistance(AstarNode *node) override;
	
private:
	std::string texturePath;
	Vector2 offset;
	Vector2 size; //sprite
	Color color;
};
