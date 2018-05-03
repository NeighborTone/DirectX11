#pragma once
#pragma warning( disable : 4005 )	//DirectXマクロ再定義
#include <d3dx11.h>
#include <d3d11.h>
#include "Device.h"
namespace DX11
{
	class Texture
	{
	private:
		
	public:
		Texture();
		~Texture();
		ID3D11SamplerState * pSampleLiner;		//サンプラー、テクスチャーからピクセルをどのように取り出すかの指定
		ID3D11ShaderResourceView* pTexture;		//テクスチャー情報
		bool Create(const char* path);
		void Draw();
		
	};
}


