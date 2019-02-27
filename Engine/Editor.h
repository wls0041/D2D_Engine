#pragma once

class Editor final
{
public:
	static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc;

public:
	Editor();
	~Editor();

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
	class Graphics *graphics;

	std::vector<class IWidget*> widgets;
	bool bInitialized;
	bool bDockspace;
};