#pragma once

class Editor final
{
public:
	static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc;

public:
	Editor();
	~Editor();

	LRESULT MessageProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void Initialize(class Context* context);
	void Render();

private:
	void BeginDockspace();
	void EndDockspace();

private:
	class Context* context;
	std::vector<class IWidget*> widgets;
	bool bInitialized;
	bool bDockspace;
};