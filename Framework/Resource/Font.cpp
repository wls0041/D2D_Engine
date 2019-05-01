#include "Framework.h"
#include "Font.h"

#define ASCII_TAB 9
#define ASCII_NEW_LINE 10
#define ASCII_SPACE 32

Font::Font(Context * context)
	: IResource(context)
	, texture_atlas(nullptr)
	, font_size(16)
	, font_color(0.0f)
	, current_text("")
	, char_max_width(0)
	, char_max_height(0)
{
	vertex_buffer = std::make_shared<VertexBuffer>(context);
	index_buffer = std::make_shared<IndexBuffer>(context);
}

Font::~Font()
{
}

void Font::SaveToFile(const std::string & filePath)
{
}

void Font::LoadFromFile(const std::string & filePath)
{
}

void Font::SetSize(const uint & font_size)
{
}

void Font::SetText(const std::string & text, const Vector2 & position)
{
}

void Font::UpdateBuffers()
{
}
