#include "stdafx.h"
#include "IconProvider.h"

void IconProvider::Initialize(conetxt * context)
{
}

Thumbnail * IconProvider::Load(const std::string & path, const IconType & type)
{
	return nullptr;
}

ID3D11ShaderResourceView * IconProvider::GetShaderResourceFromType(const IconType & type)
{
	return nullptr;
}

ID3D11ShaderResourceView * IconProvider::GetShaderResourceFromPath(const std::string & path)
{
	return nullptr;
}

const bool IconProvider::ImageButton(const IconType & type, const float & size)
{
	return false;
}

const bool IconProvider::ImageButton(const std::string & path, const float & size)
{
	return false;
}

Thumbnail * IconProvider::GetThumbnailFromType(const IconType & type)
{
	return nullptr;
}
