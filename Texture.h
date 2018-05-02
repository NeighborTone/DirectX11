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
		Texture();
	public:
		ID3D11SamplerState * pSampleLiner;		//サンプラー、テクスチャーからピクセルをどのように取り出すかの指定
		ID3D11ShaderResourceView* pTexture;		//テクスチャー情報
		bool Create();
		void Draw();
		static Texture* GetInst()
		{
			static Texture inst;
			return &inst;
		}
		~Texture();
	};
}


