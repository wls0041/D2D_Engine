#include "Framework.h"
#include "Tilemap.h"
#include "Transform.h"
#include "../../Tile/Tile.h"
#include "../../Shader/Shader.h"

Tilemap::Tilemap(Context * context, GameObject * object, Transform * transform)
	: IComponent(context, object, transform)
	, vertexBuffer(0)
	, indexBuffer(0) 
	, width(0)
	, height(0)
	, column(0)
	, row(0)
	, spacing(16)
	, adjustPosition(0)
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
}

void Tilemap::OnStop()
{
}

void Tilemap::OnDestroy()
{
}

auto Tilemap::GetTile(const uint & row, const uint & column) const -> class Tile **
{
	if (row >= this->row || column >= this->column) {
		LOG_ERROR("Invalid parameter, out of range");
		return nullptr;
	}
	return tiles;
}

auto Tilemap::GetTileSet(const uint & index) const -> Texture *
{
	if (index >= tileSets.size()) {
		LOG_ERROR("Invalid parameter");
		return nullptr;
	}

	return tileSets[index];
}

void Tilemap::AddTileSet(Texture * tileSet)
{
	if (!tileSet) {
		LOG_ERROR("Invalid parameter");
		return;
	}
	tileSets.emplace_back(tileSet);
}

void Tilemap::AddTileSet(const std::string & path)
{
	auto tileSet = context->GetSubsystem<ResourceManager>()->Load<Texture>(path);
	AddTileSet(tileSet);
}

void Tilemap::CreateTilemap(const uint & width, const uint & height, const uint & spacing)
{
	this->width = (width % spacing) == 0 ? width : width - (width % spacing);
	this->height = (height % spacing) == 0 ? height : height - (height % spacing);
	this->row = this->width / spacing;
	this->column = this->height / spacing;

	tiles = new Tile*[column];
	for (uint y = 0; y < column; y++){
		tiles[y] = new Tile[row];
		for (uint x = 0; x < row; x++) {
			tiles[y][x].SetPosition({ static_cast<float>(x * spacing), static_cast<float>(y * spacing) });
			tiles[y][x].SetScale(spacing);
		}
	}
}

void Tilemap::ClearTilemap()
{
	for (uint y = 0; y < column; y++) SAFE_DELETE_ARRAY(tiles[y]);
	SAFE_DELETE_ARRAY(tiles);
}
