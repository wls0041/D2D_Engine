#pragma once

class Pipeline final
{
public:
	Pipeline(class Context* context);
	~Pipeline();

	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;

	//=========================================================================
	//IAStage
	void SetVertexBuffer(class VertexBuffer* buffer);
	void SetIndexBuffer(class IndexBuffer* buffer);
	void SetInputLayout(class InputLayout* layout);
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology);
	//=========================================================================

	//=========================================================================
	//VSStage
	void SetVertexShader(class VertexShader* shader);
	void SetVSConstantBuffer(class ConstantBuffer* buffer);
	void SetVSShaderResource(class Texture* texture);
	void SetVSShaderResource(ID3D11ShaderResourceView* srv);
	//void SetVSSampler();
	//=========================================================================

	//=========================================================================
	//PSStage
	void SetPixelShader(class PixelShader* shader);
	void SetPSConstantBuffer(class ConstantBuffer* buffer);
	void SetPSShaderResource(class Texture* texture);
	void SetPSShaderResource(ID3D11ShaderResourceView* srv);
	//void SetPSSampler();
	//=========================================================================

	//=========================================================================
	//RSStage
	void SetRasterizerState();
	void SetViewport();
	//=========================================================================

	//=========================================================================
	//OMStage
	void SetBlendState(const BlendMode &blendMode);
	void SetDepthStencilState();
	void SetRenderTexture();
	//=========================================================================

	void BindPipeline();

	void Draw();
	void Draw(const uint& vertexCount, const uint& vertexOffset);

	void DrawIndexed();
	void DrawIndexed
	(
		const uint& indexCount,
		const uint& indexOffset,
		const uint& vertexOffset
	);

private:
	void IAStage();
	void VSStage();
	void RSStage();
	void PSStage();
	void OMStage();

private:
	void CreateBlendStates();

private:
	class Context* context;
	class Graphics* graphics;

	//IAStage
	class VertexBuffer* vertexBuffer;
	class IndexBuffer* indexBuffer;
	class InputLayout* inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

	//VSStage
	class VertexShader* vertexShader;
	std::vector<ID3D11Buffer*> vs_constantBuffers;
	std::vector<ID3D11ShaderResourceView*> vs_shaderResources;
	std::vector<ID3D11SamplerState*> vs_samplers;

	//PSStage
	class PixelShader* pixelShader;
	std::vector<ID3D11Buffer*> ps_constantBuffers;
	std::vector<ID3D11ShaderResourceView*> ps_shaderResources;
	std::vector<ID3D11SamplerState*> ps_samplers;

	//RSStage


	//OMStage
	class BlendState* blendState;

	bool bVertexBuffer;
	bool bIndexBuffer;
	bool bInputLayout;
	bool bPrimitiveTopology;
	bool bVertexShader;
	bool bPixelShader;
	bool bBlendState;

private:
	std::map<uint, BlendState*> blendStates;
};