#include "Framework.h"
#include "Texture.h"

Texture::Texture(Context * context)
	: IResource(context)
	, srv(nullptr)
	, filePath("")
	, format(DXGI_FORMAT_R8G8B8A8_UNORM)
	, width(0.0f)
	, height(0.0f)
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
	CreateTextureFromFile(filePath);
}

void Texture::CreateTextureFromFile(const std::string & filePath)
{
	this->filePath = filePath;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFileA
	(
		graphics->GetDevice(),
		filePath.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* srcTexture = nullptr;
	srv->GetResource(reinterpret_cast<ID3D11Resource**>(&srcTexture));

	D3D11_TEXTURE2D_DESC desc;
	srcTexture->GetDesc(&desc);

	width = static_cast<float>(desc.Width);
	height = static_cast<float>(desc.Height);
	format = desc.Format;
}

void Texture::CreateTexture2D(const float & width, const float & height, const DXGI_FORMAT & format)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = static_cast<uint>(width);
	texDesc.Height = static_cast<uint>(height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = graphics->GetDevice()->CreateTexture2D
	(
		&texDesc,
		nullptr,
		&texture
	);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = graphics->GetDevice()->CreateShaderResourceView
	(
		texture,
		&srvDesc,
		&srv
	);
	assert(SUCCEEDED(hr));
}

void Texture::Clear()
{
	SAFE_RELEASE(srv);
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	width = 0.0f;
	height = 0.0f;
}

void Texture::BindPipeline(const ShaderType & type, const uint & slot)
{
	auto dc = graphics->GetDeviceContext();

	switch (type)
	{
	case ShaderType::VS: dc->VSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::PS: dc->PSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::GS: dc->GSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::CS: dc->CSSetShaderResources(slot, 1, &srv); break;
	}
}
