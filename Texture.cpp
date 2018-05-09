#include "Texture.h"
#include "SystemDefs.h"
using namespace std;

Texture::Texture() :
	pTexture(nullptr),
	width(0),
	height(0)
{
}


Texture::~Texture()
{
	Memory::SafeRelease(pTexture);
}

bool Texture::Create(ID3D11Device* device, const char * path)
{
	HRESULT hr;
	
	//テクスチャー読み込み
	hr = D3DX11CreateShaderResourceViewFromFile(
		device, 
		path, 
		NULL, 
		NULL, 
		&pTexture, 
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャーの読み込みに失敗", "Error", MB_OK);
		return false;
	}

	//テクスチャーのサイズを得る
	ID3D11Resource* resource = nullptr;
	pTexture->GetResource(&resource);

	ID3D11Texture2D* texture2D = nullptr;
	hr = resource->QueryInterface(&texture2D);
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		width = desc.Width;
		height = desc.Height;
	}

	texture2D->Release();
	resource->Release();

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return pTexture;
}

string Texture::GetName()
{
	return name;
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}
