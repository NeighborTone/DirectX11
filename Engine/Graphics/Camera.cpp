#include "Camera.h"
#include "../Utilitys/Utility.hpp"
using namespace DirectX;
Camera::Camera():
	renderTargetView(nullptr),
	depthStencilView(nullptr),
	renderTexture(nullptr),
	depthTexture(nullptr),
	constantBuffer(nullptr),
	pos(0,0,0),
	angle(0, 0, 0),
	color(0.4f, 0.5f, 0.8f, 1),
	isDepthTest(false)
{
	Engine::COMInitialize();
	SetOrthographic(1.0f, -D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX);
	Create();
	System::AddProcedure(this);
}


Camera::~Camera()
{
	System::RemoveProcedure(this);
}

void Camera::SetPerspective(float fieldOfView, float nearClip, float farClip)
{
	isPerspective = true;
	this->fieldOfView = fieldOfView;
	this->nearClip = nearClip;
	this->farClip = farClip;
	float aspectRatio = Engine::GetWindowSize().x / (float)Engine::GetWindowSize().y;
	constant.projection = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearClip, farClip));
}

void Camera::SetOrthographic(float size, float nearClip, float farClip)
{
	isPerspective = false;
	this->size = size;
	this->nearClip = nearClip;
	this->farClip = farClip;
	constant.projection = XMMatrixTranspose(
		XMMatrixOrthographicLH((float)Engine::GetWindowSize().x * size, (float)Engine::GetWindowSize().y * size, nearClip, farClip));
}

void Camera::SetDepthTest(bool isDepthTest)
{
	this->isDepthTest = isDepthTest;
}

void Camera::Run(bool clearBack)
{
	//Worldに逆行列をかけてビューを作成
	//ビュー行列は平行移動行列の逆行列に回転行列の逆行列を掛け算すると求まる
	//ビューマトリックスの初期化
	constant.view = XMMatrixTranspose(
		XMMatrixInverse(
			nullptr,
			XMMatrixRotationX(XMConvertToRadians(angle.x)) *
			XMMatrixRotationY(XMConvertToRadians(angle.y)) *
			XMMatrixRotationZ(XMConvertToRadians(angle.z)) *
			XMMatrixTranslation(pos.x, pos.y, pos.z)
		)
	);

	Engine::GetDXContext3D().UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
	Engine::GetDXContext3D().VSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().HSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().DSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().GSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().PSSetConstantBuffers(0, 1, &constantBuffer.p);

	if (clearBack)
	{
		float clearColor[4] = { color.r, color.g, color.b, color.a };
		//背景色
		Engine::GetDXContext3D().ClearRenderTargetView(renderTargetView, clearColor);
	}

	if (isDepthTest)
	{
		Engine::GetDXContext3D().ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Engine::GetDXContext3D().OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}
	else
	{
		Engine::GetDXContext3D().OMSetRenderTargets(1, &renderTargetView.p, nullptr);

	}

}

bool Camera::Create()
{
	HRESULT hr;
	//先にクリアしておく
	renderTexture.Release();	

	//バックバッファーにアクセス
	Engine::GetDXSwapChain().GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&renderTexture));

	renderTargetView.Release();

	//リソースデータへのアクセス用にレンダーターゲットビューを作成
	hr = Engine::GetDXDevice3D().CreateRenderTargetView(
		renderTexture,					//ID3D11Resourceへのポインタ
		nullptr,								//D3D11_RENDER_TARGET_VIEW_DESCへのポインター
		&renderTargetView);			//ID3D11RenderTargetViewへのポインターのアドレス

	ErrorMessage(hr, "レンダーターゲットビューの作成に失敗", "Error");
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SecureZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	Engine::GetDXSwapChain().GetDesc(&swapChainDesc);
	depthTexture.Release();

	//2Dテクスチャーについて記述します
	D3D11_TEXTURE2D_DESC textureDesc;
	SecureZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = static_cast<UINT>(Engine::GetWindowSize().x);		//テクスチャーの幅です (テクセル単位)
	textureDesc.Height = static_cast<UINT>(Engine::GetWindowSize().y);		//テクスチャーの高さです (テクセル単位)
	textureDesc.MipLevels = 1;																		//テクスチャー内のミップマップレベルの最大数
	textureDesc.ArraySize = 1;																		//テクスチャー配列内のテクスチャーの数です。
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;								//テクスチャーフォーマット
	textureDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;		//ピクセル単位のマルチサンプリングの数
	textureDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;	//イメージの品質レベルです。品質が高いほど、パフォーマンスは低下します。有効な範囲は0から-1
	textureDesc.Usage = D3D11_USAGE_DEFAULT;										//テクスチャーの読み込みおよび書き込み方法を識別する値
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;							//パイプラインステージへのバインドに関するフラグ
	textureDesc.CPUAccessFlags = 0;															//許可するCPUアクセスの種類を指定するフラグ
	textureDesc.MiscFlags = 0;																		//他の一般性の低いリソースオプションを識別するフラグ

	//2Dテクスチャーの配列を作成
	hr = Engine::GetDXDevice3D().CreateTexture2D(
		&textureDesc,	//2Dテクスチャーの記述へのポインター
		nullptr,				//サブリソースの記述の配列へのポインター
		&depthTexture);	//作成されるテクスチャーへのポインターのアドレス

	ErrorMessage(hr, "2Dテクスチャー配列の作成に失敗", "Error");

	depthStencilView.Release();

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;	//リソースデータのフォーマット

	if (swapChainDesc.SampleDesc.Count == 0)
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//リソースのタイプ.リソースは2Dテクスチャーとしてアクセスされます。
		depthStencilViewDesc.Texture2D.MipSlice = 0;		//最初に使用するミップマップ レベルのインデックス
	}
	else
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;		//リソースはマルチサンプリングを使用する2Dテクスチャーとしてアクセスされます。
	}

	//リソースデータへのアクセス用に深度ステンシルビューを作成
	hr = Engine::GetDXDevice3D().CreateDepthStencilView(
		depthTexture,			 //深度ステンシルサーフェスとして機能するリソースへのポインター
		&depthStencilViewDesc,  //深度ステンシルビューの記述へのポインター
		&depthStencilView);		//ID3D11DepthStencilViewへのポインター

	ErrorMessage(hr, "深度ステンシルビューの作成に失敗", "Error");

	constantBuffer.Release();
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC constantBufferDesc;
	SecureZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Constant));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	hr = Engine::GetDXDevice3D().CreateBuffer(
		&constantBufferDesc,
		nullptr, 
		&constantBuffer);
	
	ErrorMessage(hr, "コンスタントバッファーの作成に失敗", "Error");

	return true;
}

void Camera::OnProceed(HWND, UINT message, WPARAM, LPARAM)
{
	//ウィンドウサイズが変更されたらメッセージを受け取って画面を構成しなおす
	if (message != WM_SIZE)
		return;

	if (Engine::GetWindowSize().x <= 0.0f || Engine::GetWindowSize().y <= 0.0f)
		return;

	if (isPerspective)
		SetPerspective(fieldOfView, nearClip, farClip);
	else
		SetOrthographic(size, nearClip, farClip);

	Create();
}
