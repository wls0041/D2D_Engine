#include "stdafx.h"
#include "Widget_Menubar.h"
#include "../Tool/Tool_Sprite.h"

Widget_Menubar::Widget_Menubar(Context * context)
	: IWidget(context), bShowMetricWindow(false), bShowStyleEditor(false), bShowDemoWindow(false), bShowToolSprite(false)
{
}

Widget_Menubar::~Widget_Menubar()
{
}

void Widget_Menubar::Render()
{
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) 
		{	
			if (ImGui::MenuItem("Load")) OpenFile();
			ImGui::Separator(); //���м�
			if (ImGui::MenuItem("Save")) SaveFile();
			if (ImGui::MenuItem("Save As")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) 
		{
			ImGui::MenuItem("Metrics", nullptr, &bShowMetricWindow); //2->����Ű(���� ��������)
			ImGui::MenuItem("Style", nullptr, &bShowStyleEditor);
			ImGui::MenuItem("Demo", nullptr, &bShowDemoWindow);
			ImGui::MenuItem("Sprite", nullptr, &bShowToolSprite);
		
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (bShowMetricWindow) ImGui::ShowMetricsWindow();
	if (bShowStyleEditor) ImGui::ShowStyleEditor();
	if (bShowDemoWindow) ImGui::ShowDemoWindow();
	if (bShowToolSprite) Tool_Sprite::Get().SetIsVisible(bShowToolSprite);
}

void Widget_Menubar::OpenFile(const std::string & path)
{
	if (path.length() < 1) //openfiledia�� ���� openfile(fileName)�� ����
		FileSystem::OpenFileDialog(bind(&Widget_Menubar::OpenFile, this, std::placeholders::_1));
	else {
		//TODO :
	}
}

void Widget_Menubar::SaveFile(const std::string & path)
{
	if (path.length() < 1)
		FileSystem::SaveFileDialog(bind(&Widget_Menubar::SaveFile, this, std::placeholders::_1));
	else {
		//TODO :
	}
}
