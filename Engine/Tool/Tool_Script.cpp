#include "stdafx.h"
#include "Tool_Script.h"
#include "../ImGui/TextEditor.h"

namespace _Tool_Script {
	TextEditor textEditor;
	TextEditor::ErrorMarkers markers;
}

void Tool_Script::Initialize(Context * context)
{
	this->context = context;
	this->path = "";
	this->bVisible = false;

	auto language = TextEditor::LanguageDefinition::CPlusPlus();
	{
		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::BeginMenu("File"));
			{
				if(ImGui::MenuItem("Open")) {}
				if(ImGui::MenuItem("Save")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark")) _Tool_Script::textEditor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light")) _Tool_Script::textEditor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("RetroBlue")) _Tool_Script::textEditor.SetPalette(TextEditor::GetRetroBluePalette());

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
	}
	_Tool_Script::textEditor.SetLanguageDefinition(language);
}

void Tool_Script::Render()
{
	if (!bVisible) return;

	auto cursorPos = _Tool_Script::textEditor.GetCursorPosition();

	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Script Tool", &bVisible, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	{
		_Tool_Script::textEditor.Render("Script Tool");
	}
	ImGui::End();
}

void Tool_Script::SetScript(const std::string & path)
{
	this->path = path;

	std::ifstream in(path);
	if (in.good()) {
		std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()); //공간을 미리 잡고(2), 거기에 넣음(1)
		_Tool_Script::textEditor.SetText(str);
	}
}
