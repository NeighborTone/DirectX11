#include "Texture.h"
#include "../Engine.h"
#include "../Utilitys/Utility.hpp"
#include "../Utilitys/StringConverter.hpp"
#include <memory>


Texture::Texture():
	texture(nullptr),
	shaderResourceView(nullptr),
	samplerState(nullptr)
{
	Engine::COMInitialize();
}

Texture::Texture(const std::string  filePath)
{
	Engine::COMInitialize();
	Load(filePath);
}

Texture::Texture(const BYTE * const buffer, int width, int height)
{
	Engine::COMInitialize();
	Create(buffer, width, height);
}


Texture::~Texture()
{

}


void Texture::Load(const std::string filePath)
{

	HRESULT hr;
	ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;
	
	hr = Engine::GetTextureFactory().CreateDecoderFromFilename(
		StringConverter::StringToWString(filePath).c_str(),
		0,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	Message(hr,"テクスチャーの読み込みに失敗","Error");

	ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
	decoder->GetFrame(0, &frame);
	UINT width, height;
	frame->GetSize(&width, &height);

	WICPixelFormatGUID pixelFormat;
	hr = frame->GetPixelFormat(&pixelFormat);
	Message(hr, "テクスチャーの形式取得に失敗", "Error");

	std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);

	if (pixelFormat != GUID_WICPixelFormat32bppBGRA)
	{
		//Direct2Dはビットマップしか描画できないので変換を行う
		ATL::CComPtr<IWICFormatConverter> formatConverter = nullptr;
		hr = Engine::GetTextureFactory().CreateFormatConverter(&formatConverter);
		Message(hr,"コンバーターの作成に失敗","Error");
	    
		hr =	formatConverter->Initialize(
			frame, 
			GUID_WICPixelFormat32bppBGRA, 
			WICBitmapDitherTypeErrorDiffusion, 
			0, 
			0, 
			WICBitmapPaletteTypeCustom);
		Message(hr, "テクスチャーのコンバートに失敗", "Error");

		hr = formatConverter->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		Message(hr, "テクスチャー情報の取得に失敗", "Error");
	}
	else
	{
		hr = frame->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		Message(hr, "テクスチャー情報の取得に失敗", "Error");
	}

	Create(buffer.get(), width, height);
}

void Texture::Create(const BYTE * const buffer, int width, int height)
{
	size = DirectX::XMINT2(width, height);

	// レンダリングテクスチャを作成する
	texture.Release();
	D3D11_TEXTURE2D_DESC textureDesc;
	SecureZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
	textureSubresourceData.pSysMem = buffer;
	textureSubresourceData.SysMemPitch = width * 4;
	textureSubresourceData.SysMemSlicePitch = width * height * 4;
	Engine::GetDXDevice3D().CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);

	//ShaderResourceViewを作成する
	//これを作成しておかないとテクスチャリソースとして使用できない
	shaderResourceView.Release();
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	Engine::GetDXDevice3D().CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView);

	//サンプラーの作成
	samplerState.Release();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		//テクスチャーのサンプリング時に使用するフィルタリングメソッド
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//0～1の範囲外にあるテクスチャー座標を解決するために使用されるメソッド
	samplerDesc.MipLODBias = 0.0f;													//計算されたミップマップレベルからのオフセット
	samplerDesc.MaxAnisotropy = 1;													//Filter にD3D11_FILTER_ANISOTROPICまたはD3D11_FILTER_COMPARISON_ANISOTROPICが指定されている場合に使用されるクランプ(範囲固定)値
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//既存のサンプリングデータに対してデータを比較する関数
	samplerDesc.BorderColor[0] = 0.0f;											//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[1] = 0.0f;											//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[2] = 0.0f;											//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.BorderColor[3] = 0.0f;											//AddressU、AddressV、AddressWにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用される境界の色。0.0～1.0の範囲で指定
	samplerDesc.MinLOD = 0.0f;														//アクセスをクランプするミップマップ範囲の下限です。0は最大かつ最も詳細なミップマップレベル
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                          //アクセスをクランプするミップマップ範囲の上限
	Engine::GetDXDevice3D().CreateSamplerState(&samplerDesc, &samplerState);
}

DirectX::XMINT2 Texture::GetSize() const
{
	return size;
}

void Texture::Attach(int slot)
{
	if (texture == nullptr)
	{
		return;
	}
	//テクスチャーデータをシェーダーに送る
	Engine::GetDXContext3D().PSSetShaderResources(slot, 1, &shaderResourceView.p);
	Engine::GetDXContext3D().PSSetSamplers(slot, 1, &samplerState.p);
}

ID3D11Texture2D& Texture::GetInterface()
{
	return *texture;
}
