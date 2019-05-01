#pragma once
#include "IResource.h"

struct Glyph final {
	int left;
	int right;
	int top;
	int bottom;
	int width;
	int height;
	float uv_left;
	float uv_right;
	float uv_top;
	float uv_bottom;
	int descent;
	int horizontal_offset;
};

// Glyph metrics:
// --------------
//
//                       xmin                     xmax
//                        |                         |
//                        |<-------- width -------->|
//                        |                         |
//              |         +-------------------------+----------------- ymax
//              |         |    ggggggggg   ggggg    |     ^        ^
//              |         |   g:::::::::ggg::::g    |     |        |
//              |         |  g:::::::::::::::::g    |     |        |
//              |         | g::::::ggggg::::::gg    |     |        |
//              |         | g:::::g     g:::::g     |     |        |
//    offsetX  -|-------->| g:::::g     g:::::g     |  offsetY     |
//              |         | g:::::g     g:::::g     |     |        |
//              |         | g::::::g    g:::::g     |     |        |
//              |         | g:::::::ggggg:::::g     |     |        |
//              |         |  g::::::::::::::::g     |     |      height
//              |         |   gg::::::::::::::g     |     |        |
//  baseline ---*---------|---- gggggggg::::::g-----*--------      |
//            / |         |             g:::::g     |              |
//     origin   |         | gggggg      g:::::g     |              |
//              |         | g:::::gg   gg:::::g     |              |
//              |         |  g::::::ggg:::::::g     |              |
//              |         |   gg:::::::::::::g      |              |
//              |         |     ggg::::::ggg        |              |
//              |         |         gggggg          |              v
//              |         +-------------------------+----------------- ymin
//              |                                   |
//              |------------- advanceX


class Font final : IResource
{
public:
	Font(class Context *context);
	~Font();

	Font(const Font&) = delete;
	Font &operator=(const Font&) = delete;

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	auto GetVertexBuffer() const -> VertexBuffer* { return vertex_buffer.get(); }
	auto GetIndexBuffer() const -> IndexBuffer* { return index_buffer.get(); }
	auto GetTextureAtlas() const -> Texture* { return texture_atlas.get(); }
	auto GetGlyphs() const -> const std::map<uint, Glyph>& { return glyphs; }

	auto GetColor() const -> const Color& { return font_color; }
	void SetColor(const Color &font_color) { this->font_color = font_color; }

	auto GetSize() const -> const uint& { return font_size; }
	void SetSize(const uint &font_size);

	void SetText(const std::string &text, const Vector2 &position);

private:
	void UpdateBuffers();

private:
	std::string current_text;
	Geometry<VertexTexture> geometry;
	std::shared_ptr<VertexBuffer> vertex_buffer;
	std::shared_ptr<IndexBuffer> index_buffer;
	std::shared_ptr<Texture> texture_atlas;
	uint font_size;
	uint char_max_width;
	uint char_max_height;
	Color font_color;
	std::map<uint, Glyph> glyphs;

};