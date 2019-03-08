#pragma once

class Pipeline
{
public:
	Pipeline(class Context *context);
	virtual ~Pipeline();

	Pipeline(const Pipeline &rhs) = delete;
	Pipeline &operator=(const Pipeline &rhs) = delete;

	void SetVertexBuffer(class VertexBuffer *vertexBuffer);
	void SetIndexBuffer(class IndexBuffer *indexBuffer);
	void SetInputLayout(class InputLayout *inputLayout);
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY &primitiveTopology);
	void SetVertexShader(class VertexShader *vertexShader);
	void SetPixelShader(class PixelShader *pixelShader);
	void SetVSConstantBuffer(class ConstantBuffer *constantBuffer);
	void SetPSConstantBuffer(class ConstantBuffer *constantBuffer);

	void BindPipeline();
	void Draw(const uint &vertexCount, const uint &startVertexLocation);
	void DrawIndexed(const uint &indexCount, const uint &startIndexLocation, const uint &startVertexLocation);
	
private:
	class Graphics *graphics;
	
	//IAStage
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	class InputLayout *inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

	//VSStage
	class VertexShader *vertexShader;
	std::vector<class ConstantBuffer*> vs_constantBuffers;
	std::vector<class Texture*> vs_textures;
	std::vector<class Sampler*> vs_samplers;
	
	//PSStage
	class PixelShader *pixelShader;
	std::vector<class ConstantBuffer*> ps_constantBuffers;
	std::vector<class Texture*> ps_textures;
	std::vector<class Sampler*> ps_samplers;

	//RSStage

	//OMStage

	bool bVertexBuffer;
	bool bIndexBuffer;
	bool bInputLayout;
	bool bPrimitiveTopology;
	bool bVertexShader;
	bool bPixelShader;
};