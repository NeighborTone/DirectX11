#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadShaderResource(ID3D11Device * device, HWND hwnd, const char * shaderPath, const char * vertexFuncName, const char * pixelFuncName)
{
}

void ResourceManager::LoadShaderResource(Shader * shader)
{
}

void ResourceManager::LoadTextureResource(ID3D11Device * device, const char * textureFileName)
{
}

Shader * ResourceManager::GetShaderName(char * shaderName)
{
	return nullptr;
}

Texture * ResourceManager::GetTextureName(char * textureName)
{
	return nullptr;
}
