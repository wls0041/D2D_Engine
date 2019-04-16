#include "Framework.h"
#include "Texture.h"
#include "./Importer/ImageImporter.h"

Texture::Texture(Context * context)
	: IResource(context)
	, srv(nullptr)
	, format(DXGI_FORMAT_R8G8B8A8_UNORM)
	, bpp(0)
	, bpc(0)
	, width(0)
	, height(0)
	, channels(0)
	, bGrayscale(false)
	, bTransparent(false)
	, bNeedsMipChain(true)
{
	graphics = context->GetSubsystem<Graphics>();
}

Texture::~Texture()
{
	Clear();
}

void Texture::SaveToFile(const std::string & filePath)
{
	Serialize(filePath);
}

void Texture::LoadFromFile(const std::string & filePath)
{
	auto path = FileSystem::GetRelativeFilePath(filePath);
	if (!FileSystem::Exist_File(path))
	{
		Log::WriteFormatError("Texture::LoadFromFile : Path \"%s\" is invalid", path.c_str());
		return;
	}

	ClearTextureBytes();

	bool bLoading = false;
	if (FileSystem::GetExtensionFromFilePath(path) == ".texture")
		bLoading = Deserialize(path);
	else if (FileSystem::IsSupportedImageFile(path))
		bLoading = Load(path);

	if (!bLoading)
	{
		Log::WriteFormatError("Texture::LoadFromFile : Failed to load \"%s\"", path.c_str());
		return;
	}

	if (width == 0 || height == 0 || channels == 0 || mipChain.empty() || mipChain.front().empty())
	{
		Log::Error("Texture::LoadFromFile : Invalid parameter");
		return;
	}

	HasMipChain() ?
		CreateShaderResourceView(width, height, channels, format, mipChain) :
		CreateShaderResourceView(width, height, channels, format, mipChain.front(), bNeedsMipChain);

	if (FileSystem::GetExtensionFromFilePath(path) == ".texture")
		ClearTextureBytes();
}

void Texture::GetTextureBytes(std::vector<MipLevel>* textureBytes)
{
	if (!mipChain.empty())
	{
		textureBytes = &mipChain;
		return;
	}

	auto r = std::make_unique<FileStream>(resourcePath, StreamMode::Read);
	if (!r->IsOpen())
		return;

	uint mipCount = r->Read<uint>();
	for (uint i = 0; i < mipCount; i++)
	{
		textureBytes->emplace_back(MipLevel());
		r->Read(mipChain[i]);
	}
}

void Texture::ClearTextureBytes()
{
	for (auto& mip : mipChain)
	{
		mip.clear();
		mip.shrink_to_fit();
	}

	mipChain.clear();
	mipChain.shrink_to_fit();
}

auto Texture::GetMipLevelData(const uint & index) -> MipLevel *
{
	if (index >= mipChain.size())
	{
		Log::Error("Texture::GetMipLevelData : Index out of range");
		return nullptr;
	}

	return &mipChain[index];
}

auto Texture::Load(const std::string & path) -> const bool
{
	auto importer = resourceMgr->GetImageImporter();
	if (!importer->Load(path, this))
	{
		Log::Error("Texture::Load : Failed loading texture");
		return false;
	}

	SetResourcePath(FileSystem::GetPathWithoutExtension(path) + ".texture");
	SetResourceName(FileSystem::GetIntactFileNameFromFilePath(GetResourcePath()));

	return true;
}

auto Texture::CreateShaderResourceView(const uint & width, const uint & height, const uint & channels, const DXGI_FORMAT & format, const std::vector<MipLevel>& mipChain) -> const bool
{
	if (width == 0 || height == 0 || mipChain.empty())
	{
		Log::Error("Texture::CreateShaderResourceView : Invalid parameter");
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = mipChain.size();
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;

	std::vector<D3D11_SUBRESOURCE_DATA> subResourceDatas;
	uint mipWidth = width;
	uint mipHeight = height;


	for (uint i = 0; i < mipChain.size(); i++) {
		if (mipChain[i].empty()) {
			Log::WriteFormatError("Texture::CreateShaderResourceView - Mip Levels %d has ivalid data", i);
			continue;
		}

		//원래는 만들고 집어넣지만 이렇게 먼저 집어넣고 입력할 수도 있음
		auto &subData = subResourceDatas.emplace_back(D3D11_SUBRESOURCE_DATA{});
		subData.pSysMem = mipChain[i].data();
		subData.SysMemPitch = mipWidth * channels * (bpc / 8); //가로 길이 rowByte
		subData.SysMemSlicePitch = 0;

		mipWidth = Math::Max<uint>(mipWidth / 2, 1);
		mipHeight = Math::Max<uint>(mipHeight / 2, 1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D11Texture2D* texture = nullptr;
	auto result = SUCCEEDED(graphics->GetDevice()->CreateTexture2D
	(
		&desc,
		subResourceDatas.data(),
		&texture
	));

	if (!result)
	{
		Log::Error("Texture::CreateShaderResource : Failed to create ID3D11Texture2D");
		return false;
	}

	result = SUCCEEDED(graphics->GetDevice()->CreateShaderResourceView
	(
		texture,
		&srvDesc,
		&srv
	));

	if (!result)
		Log::Error("Texture::CreateShaderResource : Failed to create ID3D11ShaderResourceView");

	SAFE_RELEASE(texture);
	return result;
}

auto Texture::CreateShaderResourceView(const uint & width, const uint & height, const uint & channels, const DXGI_FORMAT & format, const MipLevel & mipLevel, const bool & bGenerateMipChain) -> const bool
{
	if (width == 0 || height == 0 || mipLevel.empty())
	{
		Log::Error("Texture::CreateShaderResourceView : Invalid parameter");
		return false;
	}

	if (bGenerateMipChain)
	{
		if (width < 4 || height < 4)
		{
			Log::WriteFormatError("Texture::CreateShaderResourceView : MipChain won't be generated as dimension %dx%d is too small", width, height);
			return false;
		}
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;

	if (bGenerateMipChain)
	{
		desc.MipLevels = 7;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = mipLevel.data();
	subResourceData.SysMemPitch = width * channels * (bpc / 8);
	subResourceData.SysMemSlicePitch = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D11Texture2D* texture = nullptr;
	auto result = SUCCEEDED(graphics->GetDevice()->CreateTexture2D
	(
		&desc,
		bGenerateMipChain ? nullptr : &subResourceData,
		&texture
	));

	if (!result)
	{
		Log::Error("Texture::CreateShaderResource : Failed to create ID3D11Texture2D");
		return false;
	}

	result = SUCCEEDED(graphics->GetDevice()->CreateShaderResourceView
	(
		texture,
		&srvDesc,
		&srv
	));

	if (result)
	{
		if (bGenerateMipChain)
		{
			graphics->GetDeviceContext()->UpdateSubresource
			(
				texture,
				0,
				nullptr,
				mipLevel.data(),
				width * channels * (bpc / 8),
				0
			);

			graphics->GetDeviceContext()->GenerateMips(srv);
		}
	}
	else
		Log::Error("Texture::CreateShaderResource : Failed to create ID3D11ShaderResourceView");

	SAFE_RELEASE(texture);
	return result;
}

void Texture::Clear()
{
	SAFE_RELEASE(srv);
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bpc = 0;
	bpp = 0;
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

	auto w = std::make_unique<FileStream>(path, StreamMode::Write);
	if (w->IsOpen())
		return false;

	w->Write(mipChain.size());
	for (auto& mip : mipChain)
		w->Write(mip);

	w->Write(bpp);
	w->Write(bpc);
	w->Write(width);
	w->Write(height);
	w->Write(channels);
	w->Write(bGrayscale);
	w->Write(bTransparent);
	w->Write(bNeedsMipChain);
	w->Write(resourceName);
	w->Write(resourcePath);

	ClearTextureBytes();
	return true;
}

auto Texture::Deserialize(const std::string & path) -> const bool
{
	auto r = std::make_unique<FileStream>(path, StreamMode::Read);
	if (!r->IsOpen())
		return false;

	ClearTextureBytes();

	mipChain.resize(r->Read<uint>());
	for (auto& mip : mipChain)
		r->Read(mip);

	r->Read(bpp);
	r->Read(bpc);
	r->Read(width);
	r->Read(height);
	r->Read(channels);
	r->Read(bGrayscale);
	r->Read(bTransparent);
	r->Read(bNeedsMipChain);
	r->Read(resourceName);
	r->Read(resourcePath);

	return true;
}
