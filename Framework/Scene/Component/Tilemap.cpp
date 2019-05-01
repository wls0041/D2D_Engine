#include "Framework.h"
#include "Tilemap.h"
#include "Transform.h"
#include "../../Tile/Tile.h"
#include "../../Shader/Shader.h"
#include "../../PathFinder/PathFinder.h"

Tilemap::Tilemap(Context * context, GameObject * object, Transform * transform)
	: IComponent(context, object, transform)
	, width(0)
	, height(0)
	, column(0)
	, row(0)
	, spacing(16)
	, adjustPosition(0, 0, 0)
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::OnInitialize()
{
	shader = std::make_shared<Shader>(context);
	shader->AddShader(ShaderType::VS, "../../_Assets/Shader/Tile.hlsl");
	shader->AddShader(ShaderType::PS, "../../_Assets/Shader/Tile.hlsl");

	Geometry<VertexTexture> geometry;
	GeometryUtility::CreateTileQuad(geometry);

	vertexBuffer = std::make_shared<VertexBuffer>(context);
	vertexBuffer->Create(geometry.GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>(context);
	indexBuffer->Create(geometry.GetIndices());

	CreateTilemap(500, 500, 16);
}

void Tilemap::OnStart()
{
}

void Tilemap::OnUpdate()
{
	if (context->GetSubsystem<Input>()->KeyDown(KeyCode::KEY_SPACE)) {
		FindPath({ 0, 0 }, { 0, 0 });
	}
}

void Tilemap::OnStop()
{
}

void Tilemap::OnDestroy()
{
}

auto Tilemap::GetTile(const uint & row, const uint & column) const -> class Tile *
{
	if (row >= this->row || column >= this->column)
	{
		LOG_ERROR("Invalid parameter, out of range");
		return nullptr;
	}

	return &tiles[column][row];
}

void Tilemap::SetTile(const uint & row, const uint & column)
{
	auto& tile = tiles[column][row];
	tile.SetOffset(currentTileData.SpriteOffset);
	tile.SetSize(currentTileData.SpriteSize);
	tile.SetTilesetSize(currentTileData.TextureSize);
	tile.SetTilesetIndex(currentTileData.TilesetIndex);
}

auto Tilemap::GetTileSet(const uint & index) const -> Texture *
{
	if (index >= tileSets.size())
	{
		LOG_ERROR("Invalid parameter, out of range");
		return nullptr;
	}

	return tileSets[index];
}

void Tilemap::AddTileSet(Texture * tileset)
{
	if (!tileset)
	{
		LOG_ERROR("Invalid parameter");
		return;
	}

	tileSets.emplace_back(tileset);
}

void Tilemap::AddTileSet(const std::string & path)
{
	auto tileset = context->GetSubsystem<ResourceManager>()->Load<Texture>(path);

	AddTileSet(tileset);
}

void Tilemap::CreateTilemap(const uint & width, const uint & height, const uint & spacing)
{
	this->width = (width % spacing) == 0 ? width : width - (width % spacing);
	this->height = (height % spacing) == 0 ? height : height - (height % spacing);
	this->row = this->width / spacing;
	this->column = this->height / spacing;

	tiles = new Tile*[column];
	for (uint y = 0; y < column; y++)
	{
		tiles[y] = new Tile[row];
		for (uint x = 0; x < row; x++)
		{
			tiles[y][x].SetPosition({ static_cast<float>(x * spacing), static_cast<float>(y * spacing) });
			tiles[y][x].SetScale(static_cast<float>(spacing));
		}
	}

	int newX = 0, newY = 0;
	Tile *child = nullptr; //부모 자식간의 연결
	for (uint y = 0; y < column; y++) {
		for (uint x = 0; x < row; x++) {
			for (int i = -1; i < 2; i++) {
				newY = y + i;
				for (int j = -1; j < 2; j++) {
					newX = x + j;

					if (newY > -1 && newY < column && newX > -1 && newX < row) {
						child = &tiles[newY][newX];

						if (child->IsWalkable() && newX != x && newY != y) {
							tiles[y][x].AddChild(child, tiles[y][x].GetLocalDistance(child));
						}
					}
				}//for(j)
			}//for(i)
		}//for(row)
	}//for(col)

	adjustPosition = Vector3
	(
		static_cast<float>(this->width) * -0.5f,
		static_cast<float>(this->height) * -0.5f,
		0.0f
	);

	auto renderer = context->GetSubsystem<Renderer>();
	renderer->AddLine({ 0, 0 }, { 300, 300 }, Color::Red);

	//horizontal
	for (uint y = 0; y <= this->height; y += spacing) {
		renderer->AddLine({ adjustPosition.x, adjustPosition.y + y }, { adjustPosition.x + this->width, adjustPosition.y + y }, Color::Red);
	}

	//vertical
	for (uint x = 0; x <= this->width; x += spacing) {
		renderer->AddLine({ adjustPosition.x + x, adjustPosition.y }, { adjustPosition.x + x, adjustPosition.y + this->height }, Color::Red);
	}
}

void Tilemap::ClearTilemap()
{
	for (uint y = 0; y < column; y++)
		SAFE_DELETE_ARRAY(tiles[y]);
	SAFE_DELETE_ARRAY(tiles);
}

void Tilemap::FindPath(const Vector2 & start, const Vector2 & goal)
{
	std::vector<Tile*> path;
	PathFinder<Tile> pathFinder;
	pathFinder.SetStart(&tiles[0][0]);
	pathFinder.SetGoal(&tiles[column - 1][row - 1]);

	auto result = pathFinder.FindPath(path);

	for (auto &tile : path) tile->SetColor(Color::Green);

}
