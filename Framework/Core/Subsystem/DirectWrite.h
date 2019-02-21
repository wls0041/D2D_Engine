#pragma once
#include "ISubsystem.h"

struct DW_FONT
{
	std::wstring Name;
	float Size;
	DWRITE_FONT_WEIGHT Weight;
	DWRITE_FONT_STYLE Style;
	DWRITE_FONT_STRETCH Stretch;

	const bool operator==(const DW_FONT& rhs)
	{
		bool bCheck = true;
		bCheck &= Name == rhs.Name;
		bCheck &= Size == rhs.Size;
		bCheck &= Weight == rhs.Weight;
		bCheck &= Style == rhs.Style;
		bCheck &= Stretch == rhs.Stretch;
		return bCheck;
	}
};

class DirectWrite : public ISubsystem
{
public:
	DirectWrite(class Context* context);
	virtual ~DirectWrite();

	const bool Initialize() override;

	void CreateSurface();
	void DeleteSurface();

	void BeginTextDraw();
	void EndTextDraw();

	void Text
	(
		const std::wstring& text,
		const Vector2& position,
		const float& fontSize = 20.0f,
		const Color& fontColor = Color(0, 0, 0, 1),
		const std::wstring& fontName = L"±¼¸²Ã¼",
		const DWRITE_FONT_WEIGHT& fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		const DWRITE_FONT_STYLE& fontStyle = DWRITE_FONT_STYLE_NORMAL,
		const DWRITE_FONT_STRETCH& fontStretch = DWRITE_FONT_STRETCH_NORMAL
	);

private:
	ID2D1SolidColorBrush* RegisterBrush(const Color& color);
	IDWriteTextFormat* RegisterFormat
	(
		const std::wstring& fontName,
		const float& fontSize,
		const DWRITE_FONT_WEIGHT& fontWeight,
		const DWRITE_FONT_STYLE& fontStyle,
		const DWRITE_FONT_STRETCH& fontStretch
	);

	ID2D1SolidColorBrush* FindBrush(const Color& color);
	IDWriteTextFormat* FindFormat(const DW_FONT& font);

private:
	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory;
	ID2D1Device* writeDevice;
	ID2D1DeviceContext* writeDeviceContext;
	ID2D1Bitmap1* targetBitmap;

	std::vector<std::pair<Color, ID2D1SolidColorBrush*>> brushes;
	std::vector<std::pair<DW_FONT, IDWriteTextFormat*>> formats;
};