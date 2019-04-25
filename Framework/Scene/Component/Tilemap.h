#pragma once
#include "IComponent.h"

class Tilemap final : public IComponent
{
public:
	Tilemap(class Context* context, class GameObject *object, class Transform *transform);
	virtual  ~Tilemap();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

	auto GetWidth() const -> const uint& { return width; }
	auto GetHeight() const -> const uint& { return height; }
	auto GetRow() const -> const uint& { return row; }
	auto GetColumn() const -> const uint& { return column; }
	auto GetSpacing() const -> const uint& { return spacing; }
	auto GetAdjustPosition() const -> const Vector3& { return adjustPosition; }

	auto GetTiles() const -> class Tile** { return tiles; }
	auto GetTileCount() const -> const uint { return row * column; }
	auto GetTile(const uint &row, const uint &column) const -> class Tile**;

	auto GetTileSets() const -> const std::vector<class Texture*>& { return tileSets; }
	auto GetTileSet(const uint &index) const -> Texture*;
	void AddTileSet(Texture *tileSet);
	void AddTileSet(const std::string &path);

	auto GetVertexBuffer() const -> VertexBuffer* { return vertexBuffer.get(); }
	auto GetIndexBuffer() const -> IndexBuffer* { return indexBuffer.get(); }
	auto GetShader() const -> class Shader* { return shader.get(); }

	void CreateTilemap(const uint &width, const uint &height, const uint &spacing);
	void ClearTilemap();

private:
	std::shared_ptr<class Shader> shader;
	std::shared_ptr<class VertexBuffer> vertexBuffer;
	std::shared_ptr<class IndexBuffer> indexBuffer;

	uint width;
	uint height;
	uint column;
	uint row;
	uint spacing;
	Vector3 adjustPosition;

	class Tile** tiles;
	std::vector<class Texture*> tileSets;
};
