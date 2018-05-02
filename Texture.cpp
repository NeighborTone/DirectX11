#include "Texture.h"

namespace DX11
{

	Texture::Texture()
	{
		pSampleLiner = nullptr;
		pTexture = nullptr;
	}


	Texture::~Texture()
	{
		if (pSampleLiner != nullptr)
		{
			RELEASE(pSampleLiner);
		}
		if (pTexture != nullptr)
		{
			RELEASE(pTexture);
		}
	}

	bool Texture::Create()
	{
		HRESULT hr;
		hr = D3DX11CreateShaderResourceViewFromFile(
			Device::GetInstace()->pDevice,
			"test.png",
			NULL,
			NULL,
			&pTexture,
			NULL
		);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャーの読み込みに失敗", "error", S_OK);
			return false;
		}

		//サンプラー作成
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory(&samDesc, sizeof(D3D11_SAMPLER_DESC));
		samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//ラッピングモードにすると、画像が繰り返し敷き詰められるようにマッピングされる
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	//テクスチャーをラップモードでマッピング
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	//テクスチャーをラップモードでマッピング
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	//テクスチャーをラップモードでマッピング
		hr = Device::GetInstace()->pDevice->CreateSamplerState(&samDesc, &pSampleLiner);
		if (FAILED(hr))
		{
			MessageBox(NULL, "サンプラーの作成に失敗", "error", S_OK);
			return false;
		}


		
		return true;
	}

	void Texture::Draw()
	{
		//テクスチャーをシェーダーに渡す
		//サンプラーはそのテクスチャーから色をどう取り出すかの指定
		Device::GetInstace()->pDeviceContext->PSSetSamplers(
			0,	//デバイスの配列の中でサンプラーの設定を開始する位置の、0 から始まるインデックス
			1,	//配列内のサンプラーの数。最大16
			&pSampleLiner);
		Device::GetInstace()->pDeviceContext->PSSetShaderResources(
			0,	//デバイスの配列の中でシェーダー リソースの設定を開始する位置の、0 から始まるインデックス
			1,	//設定するシェーダー リソースの数。最大128
			&pTexture);
	}

}
