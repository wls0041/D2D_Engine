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

	bool &IsVisible() { return bVisible; }

public:
	Tool_Sprite() = default;
	~Tool_Sprite() = default;

	Tool_Sprite(const Tool_Sprite&) = delete;
	Tool_Sprite &operator=(const Tool_Sprite&) = delete;

private:
	void ShowSpriteFrame(const struct ImVec2 &size);

private:
	class Context *context;
	class ResourceManager *resourceMgr;
	class Texture *texture;

	bool bVisible;
};