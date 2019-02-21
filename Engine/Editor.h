#pragma once

class Editor final
{
public:
	static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc;

public:
	Editor();
	~Editor();

	LRESULT MessageProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam);
	void Resize(const uint &width, const uint &height);

	void Update();
	void Render();

private:
	void BeginDockspace();
	void EndDockspace();

	void ApplyStyle();

private:
	class Context* context;
	class Engine *engine;
	class Graphics *grapchics;

	std::vector<class IWidget*> widgets;
	bool bInitialized;
	bool bDockspace;
};