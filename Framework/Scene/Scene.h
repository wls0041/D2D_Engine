#pragma once

class Scene
{
public:
	Scene(class Context* context);
	virtual ~Scene();

	const std::string& GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	void Update();
	void Render();

private:
	class Context* context;
	std::string name;
};