#pragma once
#include "Texture.h"
class ResourceTexture
{
public:
	ResourceTexture();
	~ResourceTexture();

	bool Load(ID3D11Device* device, const char* texturePath);

	Texture* GetTexture();
	std::string GetName();

private:
	Texture* tex;
};

