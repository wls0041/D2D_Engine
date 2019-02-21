#include "Framework.h"
#include "DirectWrite.h"

DirectWrite::DirectWrite(Context * context)
	: ISubsystem(context)
	, factory(nullptr)
	, writeFactory(nullptr)
	, writeDevice(nullptr)
	, writeDeviceContext(nullptr)
	, targetBitmap(nullptr)
{
}

DirectWrite::~DirectWrite()
{
	DeleteSurface();

	SAFE_RELEASE(writeDeviceContext);
	SAFE_RELEASE(writeDevice);
	SAFE_RELEASE(factory);
	SAFE_RELEASE(writeFactory);
}

const bool  DirectWrite::Initialize()
{
	auto graphics = context->GetSubsystem<Graphics>();

	//Create Write Factory
	HRESULT hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&writeFactory)
	);
	assert(SUCCEEDED(hr));

	//Create D2D Factory
	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_NONE;

	hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&factory
	);
	assert(SUCCEEDED(hr));

	//Get D3D Information
	IDXGIDevice* dxgiDevice = nullptr;
	hr = graphics->GetDevice()->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	//Create Write Device
	hr = factory->CreateDevice(dxgiDevice, &writeDevice);
	assert(SUCCEEDED(hr));

	//Create Write Device Context
	hr = writeDevice->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&writeDeviceContext
	);
	assert(SUCCEEDED(hr));

	CreateSurface();

	SAFE_RELEASE(dxgiDevice);

	return true;
}

void DirectWrite::CreateSurface()
{
	auto graphics = context->GetSubsystem<Graphics>();

	IDXGISurface* dxgiSurface = nullptr;
	HRESULT hr = graphics->GetSwapChain()->GetBuffer
	(
		0,
		__uuidof(IDXGISurface),
		reinterpret_cast<void**>(&dxgiSurface)
	);
	assert(SUCCEEDED(hr));

	//Create D2D Backbuffer(Surface)
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	hr = writeDeviceContext->CreateBitmapFromDxgiSurface
	(
		dxgiSurface,
		&bp,
		&targetBitmap
	);
	assert(SUCCEEDED(hr));

	writeDeviceContext->SetTarget(targetBitmap);

	SAFE_RELEASE(dxgiSurface);
}

void DirectWrite::DeleteSurface()
{
	writeDeviceContext->SetTarget(nullptr);
	SAFE_RELEASE(targetBitmap);
}

void DirectWrite::BeginTextDraw()
{
	writeDeviceContext->BeginDraw();
}

void DirectWrite::EndTextDraw()
{
	HRESULT hr = writeDeviceContext->EndDraw();
	assert(SUCCEEDED(hr));
}

void DirectWrite::Text(const std::wstring & text, const Vector2 & position, const float & fontSize, const Color & fontColor, const std::wstring & fontName, const DWRITE_FONT_WEIGHT & fontWeight, const DWRITE_FONT_STYLE & fontStyle, const DWRITE_FONT_STRETCH & fontStretch)
{
	D2D1_RECT_F range;
	range.left = position.x;
	range.top = position.y;
	range.right = position.x + text.length() * fontSize;
	range.bottom = position.y + fontSize;

	auto brush = RegisterBrush(fontColor);
	auto format = RegisterFormat
	(
		fontName,
		fontSize,
		fontWeight,
		fontStyle,
		fontStretch
	);

	writeDeviceContext->DrawTextW
	(
		text.c_str(),
		text.length(),
		format,
		range,
		brush
	);
}

ID2D1SolidColorBrush * DirectWrite::RegisterBrush(const Color & color)
{
	ID2D1SolidColorBrush* brush = FindBrush(color);

	if (!brush)
	{
		D2D1::ColorF brushColor = D2D1::ColorF(color.r, color.g, color.b);
		HRESULT hr = writeDeviceContext->CreateSolidColorBrush
		(
			brushColor,
			&brush
		);
		assert(SUCCEEDED(hr));

		brushes.emplace_back(color, brush);
	}

	return brush;
}

IDWriteTextFormat * DirectWrite::RegisterFormat(const std::wstring & fontName, const float & fontSize, const DWRITE_FONT_WEIGHT & fontWeight, const DWRITE_FONT_STYLE & fontStyle, const DWRITE_FONT_STRETCH & fontStretch)
{
	DW_FONT font = 
	{
		fontName,
		fontSize,
		fontWeight,
		fontStyle,
		fontStretch
	};

	IDWriteTextFormat* format = FindFormat(font);

	if (!format)
	{
		HRESULT hr = writeFactory->CreateTextFormat
		(
			font.Name.c_str(),
			nullptr,
			font.Weight,
			font.Style,
			font.Stretch,
			font.Size,
			L"ko",
			&format
		);
		assert(SUCCEEDED(hr));

		formats.emplace_back(font, format);
	}

	return format;
}

ID2D1SolidColorBrush * DirectWrite::FindBrush(const Color & color)
{
	for (auto& brush : brushes)
	{
		if (brush.first == color)
			return brush.second;
	}
	return nullptr;
}

IDWriteTextFormat * DirectWrite::FindFormat(const DW_FONT & font)
{
	for (auto& format : formats)
	{
		if (format.first == font)
			return format.second;
	}
	return nullptr;
}
