#include "stdafx.h"
#include "EditorHelper.h"
#include "./Scene/GameObject.h"

GameObject *EditorHelper::CurrentObject = nullptr;

void EditorHelper::Initialize(Context * context)
{
	this->context = context;
	this->resourceManager = context->GetSubsystem<ResourceManager>();
	this->sceneManager = context->GetSubsystem<SceneManager>();
	this->renderer = context->GetSubsystem<Renderer>();
	this->selectScene = sceneManager->GetCurrentScene();
	this->selectObject = nullptr;
}
