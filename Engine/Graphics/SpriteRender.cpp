#include "SpriteRender.h"

//!@brief	コンストラクタ
//!@param[in]	filePath	使用する画像ファイルパス
Sprite2D::Sprite2D(const std::string& filePath)
{
	spriteBatch = new DirectX::SpriteBatch(&Engine::GetDXContext3D());

	std::wstring wstr = std::wstring(filePath.begin(), filePath.end());
	LPCWSTR sw = wstr.c_str();
	DirectX::CreateWICTextureFromFile(&Engine::GetDXDevice3D(), sw, &resource, &textureResourceView);

	rotate = 0.0f;
	pivot = DirectX::XMFLOAT2(0, 0);
}

//!@brief	デストラクタ
Sprite2D::~Sprite2D()
{
	Memory::SafeRelease(textureResourceView);
	Memory::SafeRelease(resource);
	Memory::SafeDelete(spriteBatch);
}


//!@brief	描画
//!@param[in]	draw	描画位置(x,y,width,height)
//!@param[in]	src		ソースの範囲(x,y,width,height)
//!@param[in]	color	色(r,g,b,a)
void	Sprite2D::Draw(const Math::Box2D& draw, const Math::Box2D& src, const bool isCenter, const Math::Color& color, float layer)
{

	ATL::CComPtr<ID3D11BlendState> blendState;
	blendState.Release();
	D3D11_BLEND_DESC blendDesc;
	SecureZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	//アルファブレンディングの設定
	blendDesc.RenderTarget[0].BlendEnable = true;									//ブレンディングを有効(または無効) 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;						//最初のRGBデータソースを指定
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;				//2番目のRGBデータソースを指定
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;							//RGBデータソースの組合せ方法を定義
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;						//最初のアルファデータソースを指定
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;						//2番目のアルファデータソースを指定
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;					//アルファデータソースの組合せ方法を定義
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//書き込みマスク
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Engine::GetDXDevice3D().CreateBlendState(&blendDesc, &blendState);
	Engine::GetDXContext3D().OMSetBlendState(blendState, blendFactor, 0xffffffff);

	//サンプラー
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

	DirectX::FXMVECTOR convertColor = DirectX::XMVectorSet(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
	DirectX::XMFLOAT2	pos(draw.x, draw.y);
	DirectX::XMFLOAT2	scale(draw.width / src.width, draw.height / src.height);
	if (isCenter)
	{
		pivot = DirectX::XMFLOAT2((src.x + src.width) / 2.0f, (src.y + src.height) / 2.0f);
	}
	else
	{
		pivot = DirectX::XMFLOAT2(0, 0);
	}
	DirectX::XMFLOAT2 xPos = { pos.x,pos.y };
	DirectX::XMFLOAT2 xPivot = { pivot.x,pivot.y };
	DirectX::XMFLOAT2 xScale = { scale.x , scale.y };
	spriteBatch->Begin(DirectX::SpriteSortMode_BackToFront, blendState, samplerState, nullptr, nullptr, nullptr);
	spriteBatch->SetViewport(Engine::GetViewPort());
	spriteBatch->Draw(textureResourceView, xPos, &src, convertColor, rotate, xPivot, xScale, DirectX::SpriteEffects_None, layer);
	spriteBatch->End();
}

//!@brief	回転量の調節
//!@param[in]	rotate	回転量(度数法)
void	Sprite2D::SetRotation(const float rotate)
{
	this->rotate = rotate;
}