#pragma once

class Tool_Script final
{
public:
	static Tool_Script &Get() {
		static Tool_Script instance;
		return instance;
	}

	void Initialize(class Context *context);
	void Render();

	void SetScript(const std::string &path);

	const bool &IsVisible() const { return bVisible; }
	void SetIsVisible(const bool &bVisible) { this->bVisible = bVisible; }

private:
	Tool_Script() = default;
	~Tool_Script() = default;

	Tool_Script(const Tool_Script&) = delete;
	Tool_Script &operator=(const Tool_Script&) = delete;

private:
	class Context * context;
	
	std::string path;
	bool bVisible;
};