#include "Framework.h"
#include "Texture.h"
#include "./Importer/ImageImporter.h"

Texture::Texture(Context * context)
	: IResource(context)
	, srv(nullptr)
	, format(DXGI_FORMAT_R8G8B8A8_UNORM)
	, bpp(0)
	,bpc(0)
	, width(0)
	, height(0)
	, channels(0)
	, bGrayscale(false)
	, bTransparent(false)
	, bNeedsMipChain(false)
{
	graphics = context->GetSubsystem<Graphics>();
}

Texture::~Texture()
{
	Clear();
}

void Texture::SaveToFile(const std::string & filePath)
{
}

void Texture::LoadFromFile(const std::string & filePath)
{
}

void Texture::GetTextureBytes(std::vector<MipLevel>* textureBytes)
{
}

void Texture::ClearTextureBytes()
{
	for (auto &mip : mipChain) {
		mip.clear();
		mip.shrink_to_fit();
	}
}

auto Texture::CreateShaderResourceView(const uint & width, const uint & height, const uint & channels, const DXGI_FORMAT & format, const std::vector<MipLevel>& data) -> const bool
{
	return false;
}

auto Texture::CreateShaderResourceView(const uint & width, const uint & height, const uint & channels, const DXGI_FORMAT & format, const MipLevel & data, const bool & bGenerateMipChain) -> const bool
{
	return false;
}

auto Texture::GetMipLevelData(const uint & index) -> MipLevel *
{
	return nullptr;
}

auto Texture::Load(const std::string & path) -> const bool
{
	return false;
}

void Texture::Clear()
{
	SAFE_RELEASE(srv);
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bpp = 0;
	bpc = 0;
	width = 0;
	height = 0;
	channels = 0;
	bGrayscale = false;
	bTransparent = false;
	bNeedsMipChain = false;

	ClearTextureBytes();
}

auto Texture::Serialize(const std::string & path) -> const bool
{
	GetTextureBytes(&mipChain);

	FileStream w;
	w.Open(path, StreamMode::Write);
	{
		w.Write(mipChain.size());
		for (auto &mip : mipChain) w.Write(mip);

		w.Write(bpp);
		w.Write(bpc);
		w.Write(width);
		w.Write(height);
		w.Write(channels);
		w.Write(bGrayscale);
		w.Write(bTransparent);
		w.Write(bNeedsMipChain);
		w.Write(resourceName);
		w.Write(resourcePath);
	}
	w.Close();

	ClearTextureBytes();

	return false;
}

auto Texture::Deserialize(const std::string & path) -> const bool
{
	FileStream r;

	r.Open(path, StreamMode::Read);
	{
		ClearTextureBytes();

		mipChain.resize(r.Read<uint>());
		for (auto &mip : mipChain) r.Read(mip);

		r.Read(bpp);
		r.Read(bpc);
		r.Read(width);
		r.Read(height);
		r.Read(channels);
		r.Read(bGrayscale);
		r.Read(bTransparent);
		r.Read(bNeedsMipChain);
		r.Read(resourceName);
		r.Read(resourcePath);
	}
	r.Close();

	return false;
}
