#pragma once
#include "IWidget.h"

struct Item {
	Item(const std::string &path, const Thumbnail &thumbnail) : path(path), thumbnail(thumbnail) {
		label = FileSystem::GetFileNameFromFilePath(path);
	}

	Thumbnail thumbnail;
	std::string path;
	std::string label;
};

class Widget_Assets final : public IWidget
{
public:
	Widget_Assets(class Context *context);
	~Widget_Assets() = default;

	Widget_Assets(const Widget_Assets&) = delete;
	Widget_Assets &operator=(const Widget_Assets&) = delete;

	void Render() override;

private:
	void ShowItem();
	void UpdateItems(const std::string &path);

private:
	class ResourceManager *resourceMgr;
	std::string currentPath;
	float itemSize;
	float itemSizeMin;
	float itemSizeMax;
	std::vector<Item> items;
};