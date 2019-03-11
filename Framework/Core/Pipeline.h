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
	void IAStage();
	void VSStage();
	void RSStage();
	void PSStage();
	void OMStage();

private:
	void CreateBlendStates();

private:
	class Graphics *graphics;
	
	//IAStage
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	class InputLayout *inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

	//VSStage
	class VertexShader *vertexShader;
	std::vector<ID3D11Buffer*> vs_constantBuffers;
	std::vector<ID3D11ShaderResourceView*> vs_textures;
	std::vector<ID3D11SamplerState*> vs_samplers;
	
	//PSStage
	class PixelShader *pixelShader;
	std::vector<ID3D11Buffer*> ps_constantBuffers;
	std::vector<ID3D11ShaderResourceView*> ps_textures;
	std::vector<ID3D11SamplerState*> ps_samplers;

	//RSStage

	//OMStage
	class BlendState *blendState;
	class Context * context;

	bool bVertexBuffer;
	bool bIndexBuffer;
	bool bInputLayout;
	bool bPrimitiveTopology;
	bool bVertexShader;
	bool bPixelShader;

private:
	std::map<uint, BlendState*> blendStates;
};