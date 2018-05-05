#pragma once
#include "Shader.h"

class TextureShader : public Shader 
{
private:
	//Šî’êƒNƒ‰ƒX
	typedef Shader Super;	

	ID3D11SamplerState* pSampler;
public:
	TextureShader(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName);
	~TextureShader();

	void Begin(ID3D11DeviceContext* context, int indexCount);
	
	void End(ID3D11DeviceContext* context);

protected:
	bool Load(ID3D11Device* device, HWND hwnd);

};

 