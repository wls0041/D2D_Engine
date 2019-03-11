#pragma once

class Pipeline
{
public:
	Pipeline(class Context *context);
	virtual ~Pipeline();

	Pipeline(const Pipeline &rhs) = delete;
	Pipeline &operator=(const Pipeline &) = delete;

	//IASTAGE
	void SetVertexBuffer(class VertexBuffer *buffer);
	void SetIndexBuffer(class IndexBuffer *buffer);
	void SetInputLayout(class InputLayout *layout);
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY &topology);

	//VSSTAGE
	void SetVertexShader(class VertexShader *shader);
	void SetVSConstantBuffer(class ConstantBuffer *buffer);
	void SetVSShaderResource(class Texture *texture);
	void SetVSShaderResource(ID3D11ShaderResourceView *srv);
	//void SetVSSampler();

	//PSSTAGE
	void SetPixelShader(class PixelShader *shader);
	void SetPSConstantBuffer(class ConstantBuffer *buffer);
	void SetPSShaderResource(class Texture *texture);
	void SetPSShaderResource(ID3D11ShaderResourceView *srv);
	//void SetPSSampler();

	//RSSTAGE
	void SetRasterizerState();
	void SetViewPort();

	//OMSTAGE
	void SetBlendState();
	void SetDepthStencilView();
	void SetRenderTexture();

	void BindPipeline();

	void Draw();
	void Draw(const uint &vertexCount, const uint &vertexOffset);
	
	void DrawIndexed();
	void DrawIndexed(const uint &indexCount, const uint &indexOffset, const uint &vertexOffset);
	
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
	std::vector<ID3D11ShaderResourceView*> vs_shaderResources;
	std::vector<ID3D11SamplerState*> vs_samplers;
	
	//PSStage
	class PixelShader *pixelShader;
	std::vector<ID3D11Buffer*> ps_constantBuffers;
	std::vector<ID3D11ShaderResourceView*> ps_shaderResources;
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