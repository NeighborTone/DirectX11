#include "Direct3D.h"
#include "../Engine.h"
#include "../Utilitys/Utility.hpp"
#include <array>
bool Direct3D::isFullScreen = false;
Direct3D::Direct3D(bool isFullScreen) :
	device3D(nullptr),
	context3D(nullptr),
	device2D(nullptr),
	context2D(nullptr),
	swapChain(nullptr),
	textureFactory(nullptr),
	textFactory(nullptr)
{
	this->isFullScreen =  isFullScreen;
	Engine::COMInitialize();
	Create();
}

Direct3D::~Direct3D()
{
	//これがないとフルスクリーン終了時に盛大にバグる
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}
	System::RemoveProcedure(this);
}

ID3D11Device& Direct3D::GetDevice3D() const
{
	return *device3D;
}

ID3D11DeviceContext& Direct3D::GetContext3D() const
{
	return *context3D;
}

ID2D1Device& Direct3D::GetDevice2D() const
{
	return *device2D;
}

ID2D1DeviceContext& Direct3D::GetContext2D() const
{
	return *context2D;
}

IDXGISwapChain& Direct3D::GetSwapChain() const
{
	return *swapChain;
}

IWICImagingFactory& Direct3D::GetTextureFactory() const
{
	return *textureFactory;
}

IDWriteFactory& Direct3D::GetTextFactory() const
{
	return *textFactory.Get();
}

void Direct3D::UpDate()
{
	swapChain->Present(1, 0);
}

bool Direct3D::Create()
{
	Message(IsCreateDevice(), "デバイスの作成に失敗", "Error");
	Message(IsCreateBlendState(), "アルファブレンドの設定に失敗", "Error");
	Message(IsCreateD2D(), "Direct2Dの作成に失敗", "Error");
	
	//ビューとスワップチェイン
	CreateSwapChain();
	System::AddProcedure(this);
	

	return true;
}

bool Direct3D::IsCreateDevice()
{

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//Direct2DとDirectX11を連携させるのに必須
#if defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;		//デバッグ用
#endif
	
	HRESULT hr;
	
	//ドライバータイプの列挙
	std::array<D3D_DRIVER_TYPE, 5> driverType =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE,
	};
	//DirectX11の機能レベルの設定
	D3D_FEATURE_LEVEL feature[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//デバイスとコンテキストの作成
	for (size_t i = 0; i < driverType.size(); ++i)
	{
		hr = D3D11CreateDevice(
			nullptr,					//デバイスの作成時に使用するビデオアダプターへのポインター
			driverType[i],			//作成するデバイスの種類
			nullptr,					//ソフトウェアラスタライザーを実装するDLLのハンドル
			flags,					//有効にするランタイムレイヤー
			feature,				//作成を試みる機能レベルの順序を指定するD3D_FEATURE_LEVELの配列へのポインター
			6,						//pFeatureLevelsの要素数
			D3D11_SDK_VERSION,		//SDKのバージョン。D3D11_SDK_VERSIONを指定
			&device3D,
			nullptr,
			&context3D);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Direct3D::IsCreateBlendState()
{
	HRESULT hr;
	ATL::CComPtr<ID3D11BlendState> blendState = nullptr;
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
	hr = device3D->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr))
	{
		return false;
	}
	context3D->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	return true;
}

bool Direct3D::IsCreateD2D()
{
	HRESULT hr;
	D2D1_FACTORY_OPTIONS options;
#if defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	SecureZeroMemory(&options,sizeof(D2D1_FACTORY_OPTIONS));
	
	ATL::CComPtr<ID2D1Factory1> factory = nullptr;
	
	//Direct2Dファクトリの作成
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,		//ファクトリとそのファクトリによって作成されるリソースのスレッドモデル
		options,								//デバッグレイヤーに指定された詳細のレベル
		&factory);								//このメソッドが返されるときに、新しいファクトリへのポインターのアドレスが格納される
	if (FAILED(hr))
	{
		return false;
	}
	ATL::CComPtr<IDXGIDevice> device = nullptr;
	//目的のインターフェースが使えるかを得る
	hr = device3D.QueryInterface(&device);	
	if (FAILED(hr))
	{
		return false;
	}

	//Direct2DとDirectX11で連携できるかどうか
	hr = factory->CreateDevice(device, &device2D);
	if (FAILED(hr))
	{
		return false;
	}
	//Direct2DContextの作成
	hr = device2D->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE, 
		&context2D);
	if (FAILED(hr))
	{
		return false;
	}
	//COMオブジェクトの作成
	hr = textureFactory.CoCreateInstance(
		CLSID_WICImagingFactory, 
		nullptr, 
		CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		return false;
	}
	//DirectWriteオブジェクトを作成
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory), 
		&textFactory);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Direct3D::CreateSwapChain()
{
	CComPtr<IDXGIDevice> dxgi = nullptr;
	device3D->QueryInterface(&dxgi);

	CComPtr<IDXGIAdapter> adapter = nullptr;
	dxgi->GetAdapter(&adapter);

	CComPtr<IDXGIFactory> factory = nullptr;
	adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	DXGI_SWAP_CHAIN_DESC swapDesc;
	SecureZeroMemory(&swapDesc, sizeof(swapDesc));
	//シングルバックバッファ(裏画面)を設定
	swapDesc.BufferCount = 1;

	//バックバッファのウィンドウ幅を設定
	swapDesc.BufferDesc.Width = Engine::GetWindowSize().x;
	swapDesc.BufferDesc.Height = Engine::GetWindowSize().y;

	//バックバッファを32bitに設定
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//リフレッシュレートの設定
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	//Usageの設定
	//Usageはテクスチャの性質や確保するメモリの場所を指定するオプション
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//ウィンドウハンドルの設定
	swapDesc.OutputWindow = Engine::GetWindowHandle();

	//マルチサンプリングをオフにする
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//ウィンドウモード設定
	swapDesc.Windowed = isFullScreen;

	//走査線の順序とスケーリングを不特定に設定する
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//表示済みのバックバッファの破棄
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;
	swapChain.Release();
	factory->CreateSwapChain(device3D, &swapDesc, &swapChain);
	factory->MakeWindowAssociation(Engine::GetWindowHandle(), DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
	SetViewport();
}

void Direct3D::SetViewport()
{
	//ビューポートの作成
	D3D11_VIEWPORT viewPort;
	SecureZeroMemory(&viewPort,sizeof(viewPort));
	viewPort.Width = static_cast<float>(Engine::GetWindowSize().x);
	viewPort.Height = static_cast<float>(Engine::GetWindowSize().y);
	viewPort.MaxDepth = 1.0f;
	context3D->RSSetViewports(1, &viewPort);
}

void Direct3D::OnProceed(HWND, UINT message, WPARAM, LPARAM)
{
	//ウィンドウサイズが変更されたらメッセージを受ける
	if (message != WM_SIZE)
		return;

	if (Engine::GetWindowSize().x <= 0.0f || Engine::GetWindowSize().y <= 0.0f)
		return;

	CreateSwapChain();
}
