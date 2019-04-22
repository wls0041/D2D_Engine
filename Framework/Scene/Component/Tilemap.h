#pragma once
#include "IComponent.h"

class Tilemap final : public IComponent
{
public:
	Tilemap(class Context* context, class GameObject *object, class Transform *transform);
	virtual  ~Tilemap();

	void OnInitialize() override;
	void OnStart() override; //시작하는 시점
	void OnUpdate() override;
	void OnStop() override; //멈출 때
	void OnDestroy() override;

private:
	class Grid *grid;
	class Shader *shader;

	class Geometry<VertexTexture> geometry;
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;

	class ConstantBuffer *spriteBuffer;
	class ConstantBuffer *colorBuffer;

	uint column;
	uint row;
	uint spacing;
	bool bRenderGrid;
};
