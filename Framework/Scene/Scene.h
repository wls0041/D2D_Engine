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

	class Camera* camera;
	class ConstantBuffer* cameraBuffer;
	class Rect* rect;
	class Rect* rect1;

	class AudioSource* bgm;
	std::vector<class AudioSource*> sources;
};