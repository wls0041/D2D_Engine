#include "stdafx.h"
#include "Editor.h"
#include "./ImGui/Source/imgui.h"
#include "./ImGui/Source/imgui_internal.h"
#include "./ImGui/imgui_impl_win32.h"
#include "./ImGui/imgui_impl_dx11.h"
#include "./Widget/Widget_Menubar.h"
#include "./Widget/Widget_Toolbar.h"
#include "./Widget/Widget_Hierarchy.h"
#include "./Widget/Widget_Inspector.h"
#include "./Widget/Widget_Scene.h"
#include "./Helper/EditorHelper.h"

#define DOCKING_ENABLED ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> Editor::EditorProc = nullptr;

namespace _Editor { IWidget *menuBar = nullptr; IWidget *toolBar = nullptr; }

Editor::Editor()
	: context(nullptr)
	, bInitialized(false)
	, bDockspace(true)
{
	engine = new Engine();
	context = engine->GetContext();
	graphics = context->GetSubsystem<Graphics>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcon;
	io.ConfigResizeWindowsFromEdges = true;

	ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());
	ApplyStyle();

	widgets.emplace_back(new Widget_Menubar(context));
	_Editor::menuBar = widgets.back();
	widgets.emplace_back(new Widget_Toolbar(context));
	_Editor::toolBar = widgets.back();
	widgets.emplace_back(new Widget_Hierarchy(context));
	widgets.emplace_back(new Widget_Inspector(context));
	widgets.emplace_back(new Widget_Scene(context));

	EditorProc = ImGui_ImplWin32_WndProcHandler;

	bInitialized = true;
}

Editor::~Editor()
{
	SAFE_DELETE(engine);

	if (!bInitialized) return;

	for (auto widget : widgets) SAFE_DELETE(widget);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor::Resize(const uint & width, const uint & height)
{
	if (graphics) graphics->ResizeClient(width, height);

	ImGui_ImplDX11_InvalidateDeviceObjects(); //비율, 크기가 바꼈으니 다시 그림
	ImGui_ImplDX11_CreateDeviceObjects();
}

void Editor::Update()
{
	engine->Update();
}

void Editor::Render()
{
	if (!bInitialized)
		return;

	graphics->BeginScene(); 
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (DOCKING_ENABLED) BeginDockspace();
		{
			for (auto widget : widgets)
			{
				if (widget->IsVisible())
				{
					widget->Begin();
					widget->Render();
					widget->End();
				}
			}
		}
		if (DOCKING_ENABLED) EndDockspace();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (DOCKING_ENABLED) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	graphics->EndScene();
}

void Editor::BeginDockspace()
{
	ImGuiWindowFlags windowFlags = //배경이 되는 패널
		ImGuiWindowFlags_MenuBar | 
		ImGuiWindowFlags_NoDocking | //배경이 되는 패널에 다른 imguiWindow를 도킹. 따라서 배경은 도킹 x
		//ImGuiWindowFlags_NoTitleBar |  
		ImGuiWindowFlags_NoCollapse | //최소최대 x
		ImGuiWindowFlags_NoResize | //크기조절 x
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | //포커스 x
		ImGuiWindowFlags_NoNavFocus; //tab으로 다른 창 전환 같은 네비기능 off

	ImGuiViewport *viewport = ImGui::GetMainViewport(); //위의 winplewin32에서 handle을 넘겨줘 viewport받아오면 정보를 모두 받아옴
	ImVec2 offset = ImVec2(0.0f,_Editor::menuBar->GetHeight() + _Editor::toolBar->GetHeight());

	ImGui::SetNextWindowPos(viewport->Pos + offset);
	ImGui::SetNextWindowSize(viewport->Size - offset);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); //패널 모서리 둥글기 -> 각지게하라
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); //외곽선  ->  표시하지 마라
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));//윈도우 여백 -> 여백 없애라
	ImGui::Begin("Dockspace", &bDockspace, windowFlags);
	ImGui::PopStyleVar(3); //위에서 push로 넣은 3개Var정보 삭제

	ImGuiID id = ImGui::GetID("MyDockspace"); //.ini로 저장된 설정값 가져옴
	if (!ImGui::DockBuilderGetNode(id)) { //저장된 설정이 없을 때
		ImGui::DockBuilderRemoveNode(id);
		ImGui::DockBuilderAddNode(id, ImGui::GetMainViewport()->Size);

		ImGuiID mainDock = id;
		ImGuiID leftDock = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Left, 0.2f, nullptr, &mainDock);
		ImGuiID downDock = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.6f, nullptr, &mainDock);
	
		ImGui::DockBuilderDockWindow("Scene", mainDock);
		ImGui::DockBuilderDockWindow("Hierarchy", leftDock);
		ImGui::DockBuilderDockWindow("Inspector", downDock);

		ImGui::DockBuilderFinish(mainDock);
	}
																			  //도킹 패널을 투명하게 했을 때 뒤에 다른 도킹 패널이 있다면 배경패널과 섞이지 않게 함
	ImGui::DockSpace(id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruDockspace); //0,0 -> 기본값 -> 알아서 뷰포트값 씀. 뒤의 flag값 주기 위해 넣음
}

void Editor::EndDockspace()
{
	ImGui::End();
}

void Editor::ApplyStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	float fontSize = 15.0f;
	float roundness = 2.0f;
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 text = ImVec4(0.76f, 0.77f, 0.8f, 1.0f);
	ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backgroundVeryDark = ImVec4(0.08f, 0.086f, 0.094f, 1.00f);
	ImVec4 backgroundDark = ImVec4(0.117f, 0.121f, 0.145f, 1.00f);
	ImVec4 backgroundMedium = ImVec4(0.26f, 0.26f, 0.27f, 1.0f);
	ImVec4 backgroundLight = ImVec4(0.37f, 0.38f, 0.39f, 1.0f);
	ImVec4 highlightBlue = ImVec4(0.172f, 0.239f, 0.341f, 1.0f);
	ImVec4 highlightBlueHovered = ImVec4(0.202f, 0.269f, 0.391f, 1.0f);
	ImVec4 highlightBlueActive = ImVec4(0.382f, 0.449f, 0.561f, 1.0f);
	ImVec4 barBackground = ImVec4(0.078f, 0.082f, 0.09f, 1.0f);
	ImVec4 bar = ImVec4(0.164f, 0.180f, 0.231f, 1.0f);
	ImVec4 barHovered = ImVec4(0.411f, 0.411f, 0.411f, 1.0f);
	ImVec4 barActive = ImVec4(0.337f, 0.337f, 0.368f, 1.0f);

	// Spatial
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(6, 5);
	style.Alpha = 1.0f;
	style.WindowRounding = roundness;
	style.FrameRounding = roundness;
	style.PopupRounding = roundness;
	style.GrabRounding = roundness;
	style.ScrollbarSize = 20.0f;
	style.ScrollbarRounding = roundness;

	// Colors
	style.Colors[ImGuiCol_Text] = text;
	style.Colors[ImGuiCol_WindowBg] = backgroundDark;
	style.Colors[ImGuiCol_Border] = black;
	style.Colors[ImGuiCol_FrameBg] = bar;
	style.Colors[ImGuiCol_FrameBgHovered] = highlightBlue;
	style.Colors[ImGuiCol_FrameBgActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_TitleBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_TitleBgActive] = bar;
	style.Colors[ImGuiCol_MenuBarBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_ScrollbarBg] = barBackground;
	style.Colors[ImGuiCol_ScrollbarGrab] = bar;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = barHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = barActive;
	style.Colors[ImGuiCol_CheckMark] = white;
	style.Colors[ImGuiCol_SliderGrab] = bar;
	style.Colors[ImGuiCol_SliderGrabActive] = barActive;
	style.Colors[ImGuiCol_Button] = barActive;
	style.Colors[ImGuiCol_ButtonHovered] = highlightBlue;
	style.Colors[ImGuiCol_ButtonActive] = highlightBlueActive;
	style.Colors[ImGuiCol_Header] = highlightBlue; // selected items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderHovered] = highlightBlueHovered; // hovered items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderActive] = highlightBlueActive;
	style.Colors[ImGuiCol_Separator] = backgroundLight;
	style.Colors[ImGuiCol_ResizeGrip] = backgroundMedium;
	style.Colors[ImGuiCol_ResizeGripHovered] = highlightBlue;
	style.Colors[ImGuiCol_ResizeGripActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.7f, 0.77f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = highlightBlue; // Also used for progress bar
	style.Colors[ImGuiCol_PlotHistogramHovered] = highlightBlueHovered;
	style.Colors[ImGuiCol_TextSelectedBg] = highlightBlue;
	style.Colors[ImGuiCol_PopupBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_DragDropTarget] = backgroundLight;
}
