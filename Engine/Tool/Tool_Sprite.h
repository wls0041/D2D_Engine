#pragma once

class Tool_Sprite final
{
public:
	static Tool_Sprite &Get() {
		static Tool_Sprite instance;
		return instance;
	}
	
	void Initialize(class Context *context);
	void Render();

	const bool &IsVisible() { return bVisible; }
	void SetIsVisible(const bool &bVisible) { this->bVisible = bVisible; }

public:
	Tool_Sprite() = default;
	~Tool_Sprite() = default;

	Tool_Sprite(const Tool_Sprite&) = delete;
	Tool_Sprite &operator=(const Tool_Sprite&) = delete;

private:
	void ShowSpriteFrame(const ImVec2 &size);
	void ShowSpriteEdit(const ImVec2 &size);

	void DragDropEvent();

private:
	class Context *context;
	class ResourceManager *resourceMgr;
	class Texture *texture;
	uint gridSpacing;
	Color gridColor;

	bool bGrid;
	bool bVisible;
};