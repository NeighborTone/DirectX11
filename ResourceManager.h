#pragma once
#include "ResourceShader.h"
#include "ResourceTexture.h"

#include <iostream>
#include <vector>

class ResourceManager
{
public:

	~ResourceManager();

	void LoadShaderResource(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName);
	void LoadShaderResource(Shader* shader);
	void LoadTextureResource(ID3D11Device* device, const char* textureFileName);

	Shader* GetShaderName(char* shaderName);
	Texture* GetTextureName(char* textureName);

	static ResourceManager* GetInstance();

private:
	ResourceManager();
	
	std::vector<ResourceTexture*> pTextures;
	std::vector<ResourceShader*> pShaders;

	static ResourceManager* instance;
};

