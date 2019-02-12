#pragma once

class Rect
{
public:
	Rect(class Context* context);
	virtual ~Rect();

	class Transform* GetTransform() const { return transform; }
	class Collider* GetCollider() const { return collider; }

	void Update();
	void Render();

private:
	class Context* context;
	class Graphics* graphics;

	Geometry<VertexTexture> geometry;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	InputLayout* inputLayout;
	ConstantBuffer* worldBuffer;
	Texture* texture;

	class Transform* transform;
	class Collider* collider;

	ID3D11RasterizerState* rsState;
	ID3D11BlendState* blendState;

	class Animator* animator;
	ConstantBuffer* animationBuffer;
};