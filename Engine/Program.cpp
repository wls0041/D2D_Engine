#include "stdafx.h"
#include "./Core/Window.h"
#include "./Editor.h"

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	Window::Create(hInstance, L"D2DGame", 1280, 720);
	Window::Show();

	Settings::Get().SetAppName(Window::AppName);
	Settings::Get().SetWindowInstance(Window::Instance);
	Settings::Get().SetWindowHandle(Window::Handle);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));
	Settings::Get().SetIsFullScreen(Window::IsFullScreen);
	Settings::Get().SetIsVsync(false);
	Settings::Get().SetRelative({ Settings::Get().GetWidth(), Settings::Get().GetHeight() });

	Editor *editor = new Editor();

	Window::EditorProc = Editor::EditorProc;
	Window::InputProc = Input::MouseProc;
	Window::Resize = [&editor](const uint &width, const uint &height) {
		if (editor) editor->Resize(width, height);
	};

	while (Window::Update())
	{
		editor->Update();
		editor->Render();
	}

	SAFE_DELETE(editor);
	Window::Destroy();

	return 0;
}
