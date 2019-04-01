#include "stdafx.h"
#include "Tool_Script.h"
#include "../ImGui/TextEditor.h"

const std::string Tool_Script::BaseScriptCode =
"class NewScript\n\
{\n\
\tNewScript(GameObject @inObject)\n\
\t{\n\
\t}\n\
\n\
\tvoid Start()\n\
\t{\n\
\t}\n\
\n\
\tvoid Update()\n\
\t{\n\
\t\}\n\
}";

namespace _Tool_Script
{
	TextEditor textEditor;
	TextEditor::ErrorMarkers markers;
}

void Tool_Script::Initialize(Context * context)
{
	this->context = context;
	this->path = "";
	this->bVisible = false;

	auto language = TextEditor::LanguageDefinition::AngelScript();
	//auto language = TextEditor::LanguageDefinition::CPlusPlus();
	{
		//static const char* ppnames[] =
		//{
		//   "NULL",
		//   "PM_REMOVE",
		//   "ZeroMemory",
		//   "DXGI_SWAP_EFFECT_DISCARD",
		//   "D3D_FEATURE_LEVEL",
		//   "D3D_DRIVER_TYPE_HARDWARE",
		//   "WINAPI",
		//   "D3D11_SDK_VERSION",
		//   "assert"
		//};

		//static const char* ppvalues[] =
		//{
		//   "#define NULL ((void*)0)",
		//   "#define PM_REMOVE (0x0001)",
		//   "Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ",
		//   "enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
		//   "enum D3D_FEATURE_LEVEL",
		//   "enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
		//   "#define WINAPI __stdcall",
		//   "#define D3D11_SDK_VERSION (7)",
		//   " #define assert(expression) (void)(                                                  \n"
		//   "    (!!(expression)) ||                                                              \n"
		//   "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
		//   " )"
		//};

		//for (int i = 0; i < ARRAYSIZE(ppnames); i++)
		//{
		//	TextEditor::Identifier id;
		//	id.mDeclaration = ppvalues[i];
		//	language.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
		//}

		//static const char* identifiers[] =
		//{
		//   "HWND", "HRESULT", "LPRESULT","D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC","MSG","LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		//   "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		//   "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		//   "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor"
		//};

		//static const char* idecls[] =
		//{
		//   "typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
		//   "typedef tagMSG MSG\n * Message structure","typedef LONG_PTR LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		//   "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		//   "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		//   "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor"
		//};

		//for (int i = 0; i < ARRAYSIZE(identifiers); i++)
		//{
		//	TextEditor::Identifier id;
		//	id.mDeclaration = std::string(idecls[i]);
		//	language.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
		//}
	}
	_Tool_Script::textEditor.SetLanguageDefinition(language);

	//SetScript("./ImGui/TextEditor.cpp");
	//_Tool_Script::markers.insert(std::make_pair(8, "Error!!!!!!!!!"));
	//_Tool_Script::markers.insert(std::make_pair(11, "ErrorError!!!!!!!!!"));
	//_Tool_Script::markers.insert(std::make_pair(21, "ErrorErrorError!!!!!!!!!"));
	//_Tool_Script::markers.insert(std::make_pair(24, "ErrorErrorErrorError!!!!!!!!!"));
	//_Tool_Script::textEditor.SetErrorMarkers(_Tool_Script::markers);
}

void Tool_Script::Render()
{
	if (!bVisible)
		return;

	auto cursorPos = _Tool_Script::textEditor.GetCursorPosition();

	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Script Tool", &bVisible, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) CreateNewScript();
				if (ImGui::MenuItem("Open")) {}
				if (ImGui::MenuItem("Save")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				bool bReadOnly = _Tool_Script::textEditor.IsReadOnly();
				if (ImGui::MenuItem("ReadOnly Mode", nullptr, &bReadOnly)) _Tool_Script::textEditor.SetReadOnly(bReadOnly);

				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "Ctrl + Z", nullptr, !bReadOnly && _Tool_Script::textEditor.CanUndo())) _Tool_Script::textEditor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl + Y", nullptr, !bReadOnly && _Tool_Script::textEditor.CanRedo())) _Tool_Script::textEditor.Redo();

				ImGui::Separator();
				
				if (ImGui::MenuItem("Copy", "Ctrl + C", nullptr, _Tool_Script::textEditor.HasSelection())) _Tool_Script::textEditor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl + X", nullptr, !bReadOnly && _Tool_Script::textEditor.HasSelection())) _Tool_Script::textEditor.Cut();
				if (ImGui::MenuItem("Paste", "Ctrl + V", nullptr, !bReadOnly && ImGui::GetClipboardText() != nullptr)) _Tool_Script::textEditor.Paste();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !bReadOnly && _Tool_Script::textEditor.HasSelection())) _Tool_Script::textEditor.Delete();

				ImGui::Separator();

				if (ImGui::MenuItem("Select All", "Ctrl + A")) _Tool_Script::textEditor.SelectAll();
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark"))
					_Tool_Script::textEditor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light"))
					_Tool_Script::textEditor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro Blue"))
					_Tool_Script::textEditor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text
		(
			"%6d/%-6d %6d line | %s | %s | %s | %s",
			cursorPos.mLine + 1,
			cursorPos.mColumn + 1,
			_Tool_Script::textEditor.GetTotalLines(),
			_Tool_Script::textEditor.IsOverwrite() ? "Ovr" : "Ins",
			_Tool_Script::textEditor.CanUndo() ? "*" : " ",
			_Tool_Script::textEditor.GetLanguageDefinition().mName.c_str(),
			path.c_str()
		);
		_Tool_Script::textEditor.Render("Script Tool");
	}
	ImGui::End();
}

void Tool_Script::SetScript(const std::string & path)
{
	this->path = path;

	std::ifstream in(path);
	if (in.good())
	{
		std::string str
		(
			(std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>()
		);
		_Tool_Script::textEditor.SetText(str);
	}
}

void Tool_Script::CreateNewScript()
{
	std::ofstream out;
	out.open("../../_Assets/Script/NewScript.as", std::ios::out);
	out << BaseScriptCode << std::endl;
	out.close();

	SetScript("../../_Assets/Script/NewScript.as");
}
