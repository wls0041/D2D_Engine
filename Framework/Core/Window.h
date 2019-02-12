#pragma once
#include "Framework.h"

namespace Window
{
	static HINSTANCE Instance;
	static HWND Handle;
	static std::wstring AppName;
	static bool IsFullScreen;
	static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc;
	static std::function<LRESULT(uint, WPARAM, LPARAM)> InputProc;
	static std::function<void(const uint&, const uint&)> Resize;

	inline LRESULT CALLBACK WndProc
	(
		HWND handle,
		uint message,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		if (EditorProc != nullptr)
			EditorProc(handle, message, wParam, lParam);

		if (InputProc != nullptr)
			InputProc(message, wParam, lParam);

		switch (message)
		{
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
			if (Resize != nullptr && wParam != SIZE_MINIMIZED)
				Resize(lParam & 0xffff, (lParam >> 16) & 0xffff);
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}

		return 0;
	}

	inline void Create
	(
		HINSTANCE hInstance,
		const std::wstring& appName, 
		const uint& width,
		const uint& height,
		const bool& bFullScreen = false
	)
	{
		Instance = hInstance;
		AppName = appName;
		IsFullScreen = bFullScreen;

		//1. ������ Ŭ���� ���
		WNDCLASSEX wndClass;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
		wndClass.hInstance = hInstance;
		wndClass.lpfnWndProc = static_cast<WNDPROC>(WndProc);
		wndClass.lpszClassName = appName.c_str();
		wndClass.lpszMenuName = nullptr;
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.cbSize = sizeof(WNDCLASSEX);

		WORD wHr = RegisterClassEx(&wndClass);
		assert(wHr != 0);

		if (bFullScreen)
		{
			DEVMODE devMode;
			ZeroMemory(&devMode, sizeof(DEVMODE));

			devMode.dmSize = sizeof(DEVMODE);
			devMode.dmPelsWidth = static_cast<DWORD>(Settings::Get().GetWidth());
			devMode.dmPelsHeight = static_cast<DWORD>(Settings::Get().GetHeight());
			devMode.dmBitsPerPel = 32;
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		}

		DWORD option = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		if (bFullScreen)
			option |= WS_POPUP;
		else
			option |= WS_OVERLAPPEDWINDOW;

		//2. ������ ����
		Handle = CreateWindowEx
		(
			WS_EX_APPWINDOW,
			appName.c_str(),//Ŭ������
			appName.c_str(),//Ÿ��Ʋ��
			option,
			CW_USEDEFAULT,//x
			CW_USEDEFAULT,//y
			CW_USEDEFAULT,//width
			CW_USEDEFAULT,//height
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);
		assert(Handle != nullptr);

		//3. ������ �����ֱ�
		RECT rect;
		ZeroMemory(&rect, sizeof(RECT));

		rect.left = 0;
		rect.top = 0;
		rect.right = static_cast<LONG>(width);
		rect.bottom = static_cast<LONG>(height);

		uint centerX
			= static_cast<uint>((GetSystemMetrics(SM_CXSCREEN) - width) * 0.5f);
		uint centerY
			= static_cast<uint>((GetSystemMetrics(SM_CYSCREEN) - height) * 0.5f);

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow
		(
			Handle,
			centerX,
			centerY,
			(rect.right - rect.left),
			(rect.bottom - rect.top),
			TRUE
		);
	}

	inline void Show()
	{
		SetForegroundWindow(Handle);
		SetFocus(Handle);
		ShowCursor(TRUE);
		ShowWindow(Handle, SW_SHOWNORMAL);
		UpdateWindow(Handle);
	}

	inline const bool Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;
	}

	inline void Destroy()
	{
		if (IsFullScreen)
			ChangeDisplaySettings(nullptr, 0);

		DestroyWindow(Handle);
		UnregisterClass(AppName.c_str(), Instance);
	}

	inline const uint GetWidth()
	{
		RECT rect;
		GetClientRect(Handle, &rect);
		return static_cast<uint>(rect.right - rect.left);
	}

	inline const uint GetHeight()
	{
		RECT rect;
		GetClientRect(Handle, &rect);
		return static_cast<uint>(rect.bottom - rect.top);
	}
}
