#pragma once
#include <d3d11_1.h>
#include <D3DX11tex.h>
#include <string>

class Texture
{
private:
	ID3D11ShaderResourceView * pTexture;
	std::string name;
	int width;
	int height;

public:
	Texture();
	~Texture();

	bool Create(ID3D11Device* device, const char* path);

	ID3D11ShaderResourceView* GetTexture();
	std::string GetName();

	int GetWidth();
	int GetHeight();

};

