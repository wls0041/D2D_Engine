#pragma once
#include "IResource.h"

class Material final : public IResource
{
public: 
	//���׸����� ������ ��ü���� �ٸ� ���׸����� ������ �ϹǷ� ��������� ������ �־�� ��
	//��������� �������ϸ� ���Կ����ڵ� ������ �ϴ°� ����. ���⼭�� �����ϸ鼭 �̹� ������ ���ְ� �־� ����.
	Material(class Context* context);
	Material(const Material &rhs);
	~Material();


	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	class VertexShader* GetVertexShader() const { return vertexShader; }
	class PixelShader* GetPixelShader() const { return pixelShader; }
	class InputLayout* GetInputLayout() const { return inputLayout; }
	class Texture* GetDiffuseTexture() const { return diffuseTexture; }
	const Color& GetDiffuseColor() const { return diffuseColor; }

	void SetShader(const std::string& filePath);
	void SetVertexShader(class VertexShader* vertexShader) { this->vertexShader = vertexShader; }
	void SetPixelShader(class PixelShader* pixelShader) { this->pixelShader = pixelShader; }
	void SetInputLayout(class InputLayout* inputLayout) { this->inputLayout = inputLayout; }
	void SetDiffuseTexture(class Texture* diffuseTexture) { this->diffuseTexture = diffuseTexture; }
	void SetDiffuseTexture(const std::string &textureName);
	void SetDiffuseColor(const Color& diffuseColor) { this->diffuseColor = diffuseColor; }

private:
	class VertexShader *vertexShader;
	class PixelShader *pixelShader;
	class InputLayout *inputLayout;
	class Texture *diffuseTexture;
	Color diffuseColor;
};