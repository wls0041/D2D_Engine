#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "Framework.h"
#pragma comment(lib, "Framework.lib")

#include "./ImGui/Source/imgui.h"
#include "./ImGui/Source/imgui_internal.h"
#include "./ImGui/Source/imgui_stdlib.h"

#include "./Helper/EditorHelper.h"
#include "./Helper/IconProvider.h"
#include "./Helper/DragDrop.h"
#include "./Helper/Gizmo.h"

#include "./Tool/Tool_Sprite.h"
#include "./Tool/Tool_Script.h"