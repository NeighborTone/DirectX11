#include "TextureShader.h"

TextureShader::TextureShader(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName) :
	Shader(device,hwnd,shaderPath,vertexFuncName,pixelFuncName)
{
}

TextureShader::~TextureShader()
{
	if (pSampler)
	{
		RELEASE(pSampler);
	}
}

bool TextureShader::Load(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName)
{
	if (!Super::Load(device, hwnd, shaderPath, vertexFuncName, pixelFuncName))
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		//テクスチャーのサンプリング時に使用するフィルタリングメソッド(今回は縮小、拡大にポイントサンプリングを使用し、ミップレベルのサンプリングに線形補間を使用)
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.MipLODBias = 0.0f;										//計算されたミップマップレベルからのオフセット
	samplerDesc.MaxAnisotropy = 1;										//Filter にD3D11_FILTER_ANISOTROPICまたはD3D11_FILTER_COMPARISON_ANISOTROPICが指定されている場合に使用されるクランプ(範囲固定)値
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//既存のサンプリングデータに対してデータを比較する関数
	samplerDesc.BorderColor[0] = 0;										//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[1] = 0;										//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[2] = 0;										//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[3] = 0;										//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.MinLOD = 0;													//アクセスをクランプするミップマップ範囲の下限です。0は最大かつ最も詳細なミップマップレベル
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;							//アクセスをクランプするミップマップ範囲の上限

	//テクスチャーのサンプリング情報をカプセル化するサンプラーステートの作成
	hr = device->CreateSamplerState(&samplerDesc, &pSampler);
	if (FAILED(hr))
	{
		MessageBox(NULL, "サンプラーステートの作成に失敗", "Error", MB_OK);
		return false;
	}

	return true;
}

void TextureShader::Begin(ID3D11DeviceContext* context, int indexCount)
{
	context->PSSetSamplers(0, 1, &pSampler);
	Super::Begin(context, indexCount);
}

void TextureShader::End(ID3D11DeviceContext* context)
{
	context->PSSetSamplers(0, 0, NULL);
	Super::End(context);
}

