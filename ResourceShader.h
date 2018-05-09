#pragma once
#include "Shader.h"

class ResourceShader
{
public:
	ResourceShader();
	~ResourceShader();

	bool Load(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName);
	bool Load(Shader* shader);

	Shader* GetShader();
	std::string GetName();
private:
	Shader * pShader;

};

