#include "stdafx.h"
#include "Editor.h"
#include "./ImGui/Source/imgui.h"
#include "./ImGui/imgui_impl_win32.h"
#include "./ImGui/imgui_impl_dx11.h"
#include "./Widget/Hierarchy.h"

#define DOCKING_ENABLED ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> Editor::EditorProc = nullptr;

Editor::Editor()
	: context(nullptr)
	, bInitialized(false)
	, bDockspace(true)
{
}

Editor::~Editor()
{
	if (!bInitialized)
		return;

	for (auto widget : widgets)
		SAFE_DELETE(widget);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

LRESULT Editor::MessageProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
}

void Editor::Resize()
{
}


void Editor::Initialize(Context * context)
{
	if (bInitialized)
		return;

	this->context = context;
	auto graphics = context->GetSubsystem<Graphics>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcon;
	io.ConfigResizeWindowsFromEdges = true;

	ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());

	widgets.emplace_back(new Hierarchy(context));

	EditorProc = std::bind
	(
		&Editor::MessageProc,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4
	);

	bInitialized = true;
}

void Editor::Render()
{
	if (!bInitialized)
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto widget : widgets)
	{
		if (widget->IsVisible())
		{
			widget->Begin();
			widget->Render();
			widget->End();
		}
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Editor::BeginDockspace()
{
}

void Editor::EndDockspace()
{
}
