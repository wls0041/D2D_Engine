#include "Framework.h"
#include "Settings.h"

Settings::Settings()
	: appName(L"")
	, hInstance(nullptr)
	, handle(nullptr)
	, width(0)
	, height(0)
	, bVsync(true)
	, bFullScreen(false)
	, relative(0)
{
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}
