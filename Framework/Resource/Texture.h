#pragma once
#include "IResource.h"

class Texture : public IResource
{
public:
	typedef std::vector<std::byte> MipLevel;

public:
	Texture(class Context* context);
	virtual ~Texture();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	auto GetShaderResourceView() const -> ID3D11ShaderResourceView* { return srv; }

	auto GetFormat() const -> const DXGI_FORMAT& { return format; }
	void SetFormat(const DXGI_FORMAT& format) { this->format = format; }

	auto GetBPP() const -> const uint& { return bpp; }
	void SetBPP(const uint& bpp) { this->bpp = bpp; }

	auto GetBPC() const -> const uint& { return bpc; }
	void SetBPC(const uint& bpc) { this->bpc = bpc; }

	auto GetWidth() const -> const uint& { return width; }
	void SetWidth(const uint& width) { this->width = width; }

	auto GetHeight() const -> const uint& { return height; }
	void SetHeight(const uint& height) { this->height = height; }

	auto GetChannels() const -> const uint& { return channels; }
	void SetChannels(const uint& channels) { this->channels = channels; }

	auto IsGrayscale() const -> const bool& { return bGrayscale; }
	void SetIsGrayscale(const bool& bGrayscale) { this->bGrayscale = bGrayscale; }

	auto IsTransparent() const -> const bool& { return bTransparent; }
	void SetIsTransparent(const bool& bTransparent) { this->bTransparent = bTransparent; }

	auto IsNeedsMipChain() const -> const bool& { return bNeedsMipChain; }
	void SetIsNeedsMipChain(const bool& bNeedsMipChain) { this->bNeedsMipChain = bNeedsMipChain; }

	void GetTextureBytes(std::vector<MipLevel>* textureBytes);
	void SetTextureBytes(const std::vector<MipLevel>& textureBytes) { this->mipChain = textureBytes; }
	void ClearTextureBytes();

	auto GetMipLevelData(const uint& index)->MipLevel*;
	auto AddMipLevelData() -> MipLevel* { return &mipChain.emplace_back(MipLevel()); }

	auto GetMipChain() -> const std::vector<MipLevel>& { return mipChain; }

	auto HasMipChain() -> const bool { return !mipChain.empty(); }

	auto Load(const std::string& path) -> const bool;

	auto CreateShaderResourceView //mipChain이 통째로 들어옴
	(
		const uint& width,
		const uint& height,
		const uint& channels,
		const DXGI_FORMAT& format,
		const std::vector<MipLevel>& mipChain
	) -> const bool;

	auto CreateShaderResourceView //원본 데이터만 들어옴
	(
		const uint& width,
		const uint& height,
		const uint& channels,
		const DXGI_FORMAT& format,
		const MipLevel& mipLevel,
		const bool& bGenerateMipChain = false
	) -> const bool;

	void Clear();

private:
	auto Serialize(const std::string& path) -> const bool;
	auto Deserialize(const std::string& path) -> const bool;

private:
	class Graphics* graphics;
	ID3D11ShaderResourceView* srv;
	DXGI_FORMAT format;
	uint bpp;
	uint bpc;
	uint width;
	uint height;
	uint channels;
	bool bGrayscale;
	bool bTransparent;
	bool bNeedsMipChain;
	std::vector<MipLevel> mipChain;
};