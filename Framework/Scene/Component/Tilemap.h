#pragma once
#include "IComponent.h"

class Tile {
	std::string texturePath;
	Vector2 offset;
	Vector2 size;
	Vector2 position;
	Vector2 scale;
	Color color;
};

class Tilemap final : public IComponent
{
public:
	Tilemap(class Context* context, class GameObject *object, class Transform *transform);
	virtual  ~Tilemap();

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

private:

};
