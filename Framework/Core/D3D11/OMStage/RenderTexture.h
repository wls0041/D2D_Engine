#pragma once
#include "../IPipeline.h"

class RenderTexture : public IPipeline
{
public:
	RenderTexture(class Context* context);
	virtual ~RenderTexture();

	ID3D11ShaderResourceView* GetShaderResourceView() const { return srv; }
	const DXGI_FORMAT GetFormat() const { return format; }
	const uint &GetWidth() const { return width; }
	const uint &GetHeight() const { return height; }

	void Create
	(
		const uint& width,
		const uint& height,
		const DXGI_FORMAT& format = DXGI_FORMAT_R8G8B8A8_UNORM
	);
	void Clear();

	void ClearTarget(const Color& color = 0xff555566);
	void SetTarget();
	void Save(const std::string& filePath);

private:
	class Graphics* graphics;

	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	D3D11_VIEWPORT viewport;

	DXGI_FORMAT format;
	uint width;
	uint height;
};