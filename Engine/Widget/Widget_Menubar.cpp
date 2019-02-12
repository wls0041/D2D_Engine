#include "stdafx.h"
#include "Widget_Menubar.h"

Widget_Menubar::Widget_Menubar(Context * context)
	: IWidget(context), bShowMetricWindow(false), bShowStyleEditor(false), bShowDemoWindow(false)
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
			if(ImGui::MenuItem("Load")) {}
			ImGui::Separator(); //���м�
			if (ImGui::MenuItem("Save")) {}
			if (ImGui::MenuItem("Save As")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) 
		{
			ImGui::MenuItem("Metrics", nullptr, &bShowMetricWindow); //2->����Ű(���� ��������)
			ImGui::MenuItem("Style", nullptr, &bShowStyleEditor);
			ImGui::MenuItem("Demo", nullptr, &bShowDemoWindow);
		
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (bShowMetricWindow) ImGui::ShowMetricsWindow();
	if (bShowStyleEditor) ImGui::ShowStyleEditor();
	if (bShowDemoWindow) ImGui::ShowDemoWindow();
}
