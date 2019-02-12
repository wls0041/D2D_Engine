#include "stdafx.h"
#include "./Core/Window.h"
#include "./Core/Engine.h"
#include "./Editor.h"

static Editor* editor;
static Engine* engine;
static Graphics* graphics;

void Initialize();
void Resize(const uint&, const uint&);
void Destroy();

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

	Initialize();

	while (Window::Update())
	{
		engine->Update();

		graphics->BeginScene();
		{
			editor->Render();
		}
		graphics->EndScene();
	}

	Destroy();

	Window::Destroy();

	return 0;
}

void Initialize()
{
	Settings::Get().SetAppName(Window::AppName);
	Settings::Get().SetWindowInstance(Window::Instance);
	Settings::Get().SetWindowHandle(Window::Handle);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));
	Settings::Get().SetIsFullScreen(Window::IsFullScreen);
	Settings::Get().SetIsVsync(false);

	engine = new Engine();
	engine->Initialize();

	editor = new Editor();
	editor->Initialize(engine->GetContext());

	graphics = engine->GetContext()->GetSubsystem<Graphics>();

	Window::EditorProc = Editor::EditorProc;
	Window::InputProc = Input::MouseProc;
	Window::Resize = Resize;
}

void Resize(const uint & width, const uint & height)
{
}

void Destroy()
{
	SAFE_DELETE(engine);
}
