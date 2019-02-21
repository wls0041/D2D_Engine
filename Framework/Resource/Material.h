#pragma once
#include "IResource.h"

class Material final : public IResource
{
public: 
	//매테리얼을 가지는 객체마다 다른 매테리얼을 가져야 하므로 복사생성자 가지고 있어야 함
	//복사생성자 재정의하면 대입연산자도 재정의 하는게 정석. 여기서는 생성하면서 이미 대입을 해주고 있어 생략.
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