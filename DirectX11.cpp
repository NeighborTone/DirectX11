#include "DirectX11.h"
#include "SystemHelp.h"

DirectX11::DirectX11() :
	pSwapChain(nullptr),
	pDevice(nullptr),
	pDeviceContext(nullptr),
	pRenderTargetView(nullptr),
	pDepthStencilBuffer(nullptr),
	pDepthStencilState(nullptr),
	pDepthStencilView(nullptr),
	pRasterState(nullptr),
	pAlphaEnableBlendingState(nullptr),
	pAlphaDisableBlendingState(nullptr),
	pDepthDisabledStencilState(nullptr)
{

}
DirectX11::~DirectX11()
{
	SystemHelp::SafeRelease(pDevice);
	SystemHelp::SafeRelease(pDeviceContext);
	SystemHelp::SafeRelease(pRenderTargetView);
	SystemHelp::SafeRelease(pDepthStencilBuffer);
	SystemHelp::SafeRelease(pDepthStencilState);
	SystemHelp::SafeRelease(pDepthStencilView);
	SystemHelp::SafeRelease(pRasterState);
	SystemHelp::SafeRelease(pAlphaEnableBlendingState);
	SystemHelp::SafeRelease(pAlphaDisableBlendingState);
	SystemHelp::SafeRelease(pDepthDisabledStencilState);

}
//public-------------------------------------------------------------------------------------------------------------------------
bool  DirectX11::Create(int width, int height, bool vsync, HWND hwnd, bool isfullscreen)
{
	HRESULT hr;
	//DirectXグラフィックスインフラストラクチャー(DXGI)
	IDXGIFactory* factory;			//フルスクリーン切り替えを処理する
	IDXGIAdapter* adapter;			//ディスプレイ表示を行なうためのデバイス(ディスプレイサブシステム)
	IDXGIOutput* adapterOutput;		//モニターなどに出力するためのインターフェース

	unsigned int
		numModes,
		numerator = 0,
		denominator = 0,
		stringLength;

	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	ID3D11Texture2D* backBufferPtr;

	//垂直同期設定
	vsyncEnabled = vsync;

	//DirectXGraphicインターフェイスの作成
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectXGraphicインターフェイスの作成に失敗", "Error", MB_OK);
		return false;
	}

	//DirectXGraphicインターフェイスファクトリーの作成
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectXGraphicインターフェイスファクトリーの作成に失敗", "Error", MB_OK);
		return false;
	}

	//プライマリアダプタの出力を列挙する
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		MessageBox(NULL, "プライマリアダプタの出力に失敗", "Error", MB_OK);
		return false;
	}

	//要求されたディスプレイのフォーマットに適合するフォーマットの数を取得
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,		//カラーフォーマット
		DXGI_ENUM_MODES_INTERLACED,		//オプション
		&numModes,								//モード数が返る
		NULL);									//表示リモードストへのポインターNULL に設定すると、表示モードの数を取得できる
	if (FAILED(hr))
	{
		MessageBox(NULL, "ディスプレイフォーマットの取得に失敗", "Error", MB_OK);
		return false;
	}
	//使用しているモニターとビデオカードで使用可能なモードを保持するリストを作成する
	displayModeList = new DXGI_MODE_DESC[numModes];

	//取得した表示モードリストでモードを指定する
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ディスプレイフォーマットの指定に失敗", "Error", MB_OK);
		return false;
	}

	for (int i = 0; i < (int)numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)width)
		{
			if (displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//垂直同期が失敗する環境だとここで引っかかる
	if (numerator == 0 && denominator == 0)
	{
		//MessageBox(NULL, "垂直同期に失敗", "warning", S_OK);
		numerator = 60;
		denominator = 1;
	}

	//アダプター(またはビデオカード)のDXGI1.0記述を取得
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		MessageBox(NULL, "アダプターの取得に失敗", "Error", MB_OK);
		return false;
	}

	//ビデオカードのメモリをメガバイト単位で格納する
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//ビデオカードの名前を文字配列に変換する
	error = wcstombs_s(
		&stringLength,					//変換された文字数。
		videoCardDescription,		//変換されたマルチバイト文字の文字列を生成されるバッファーのアドレス。
		128,								//サイズのバイト数
		adapterDesc.Description,	//変換するワイド文字の文字列へのポインター。
		128);								//格納されるバイトの最大数、
	if (error != 0)
	{
		MessageBox(NULL, "ビデオカードが取得できませんでした", "Error", MB_OK);
		return false;
	}

	//もういらないので破棄
	SystemHelp::SafeDeleteArr(displayModeList);
	SystemHelp::SafeRelease(adapterOutput);
	SystemHelp::SafeRelease(adapter);
	SystemHelp::SafeRelease(factory);

	if (!InitSwapChain(hwnd, isfullscreen, width, height, numerator, denominator))
	{
		return false;
	}

	//バックバッファーへのポインターを得る
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(hr))
	{
		MessageBox(NULL, "バックバッファーの参照に失敗", "Error", MB_OK);
		return false;
	}

	//レンダーターゲットビューの作成
	hr = pDevice->CreateRenderTargetView(backBufferPtr, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "レンダーターゲットビューの作成に失敗", "Error", MB_OK);
		return false;
	}
	//バックバッファの破棄
	SystemHelp::SafeRelease(backBufferPtr);

	if (!InitDepthBuffer(width, height))
	{
		return false;
	}

	if (!InitDepthStencilBuffer())
	{
		return false;
	}

	if (!InitStencilView())
	{
		return false;
	}

	//レンダーターゲットビューとデプスステンシルバッファを出力レンダリングパイプラインにバインドする
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	if (!InitRasterizerState())
	{
		return false;
	}

	InitViewport(width, height);

	if (!InitAlphaBlending())
	{
		return false;
	}

	if (!InitZBuffer())
	{
		return false;
	}

	return true;

}

void  DirectX11::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	//背景クリア色
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;


	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void DirectX11::EndScene()
{
	if (vsyncEnabled)
	{
		//60fpsで描画
		pSwapChain->Present(1, 0);
		//30fps
		//pSwapChain->Present(2, 0);
	}
	else
	{
		//垂直同期をとらないで描画
		pSwapChain->Present(0, 0);
	}
}

void  DirectX11::EnableAlphaBlending(bool enable)
{
	//アルファ値が有効か無効かで処理を変える
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;

	if (enable)
	{
		pDeviceContext->OMSetBlendState(pAlphaEnableBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState, blendFactor, 0xffffffff);
	}
}
void  DirectX11::EnableZBuffer(bool enable)
{
	//Zバッファーが有効か無効かで処理を変える
	if (enable)
	{
		pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	}
	else
	{
		pDeviceContext->OMSetDepthStencilState(pDepthDisabledStencilState, 1);
	}
}

ID3D11Device*  DirectX11::GetDevice()
{
	return pDevice;
}
ID3D11DeviceContext*  DirectX11::GetDeviceContext()
{
	return pDeviceContext;
}


//private------------------------------------------------------------------------------------------------------------------------
bool DirectX11::InitSwapChain(HWND hwnd, bool isfullscreen, int width, int height, unsigned  int numerator, unsigned int denominator)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr;

	SecureZeroMemory(&swapDesc, sizeof(swapDesc));

	//シングルバックバッファ(裏画面)を設定
	swapDesc.BufferCount = 1;

	//バックバッファのウィンドウ幅を設定
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;

	//バックバッファを32bitに設定
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//リフレッシュレートの設定
	if (vsyncEnabled)
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Usageの設定
	//Usageはテクスチャの性質や確保するメモリの場所を指定するオプション
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//ウィンドウハンドルの設定
	swapDesc.OutputWindow = hwnd;

	//マルチサンプリングをオフにする
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//ウィンドウモード設定
	swapDesc.Windowed = !isfullscreen;

	//走査線の順序とスケーリングを不特定に設定する
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//表示済みのバックバッファの破棄
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapDesc.Flags = 0;

	//DirectX11の機能レベルの設定
	D3D_FEATURE_LEVEL feature[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//スワップチェイン(フロントバッファ)とコンテキストの作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,									//デバイスの作成時に使用するビデオアダプターへのポインター
		D3D_DRIVER_TYPE_HARDWARE,	//作成するデバイスの種類
		NULL,									//ソフトウェアラスタライザーを実装するDLLのハンドル
		0,										//有効にするランタイムレイヤー
		feature,								//作成を試みる機能レベルの順序を指定するD3D_FEATURE_LEVELの配列へのポインター
		6,										//pFeatureLevelsの要素数
		D3D11_SDK_VERSION,				//SDKのバージョン。D3D11_SDK_VERSIONを指定
		&swapDesc,							//スワップチェーンの初期化パラメーターを格納するスワップチェーンの記述へのポインター
		&pSwapChain,						//レンダリングに使用するスワップ チェーンを表すIDXGISwapChainオブジェクトへのポインターのアドレスを返す
		&pDevice,							//作成されたデバイスを表すID3D11Deviceオブジェクトへのポインターのアドレスを返す。NULLを指定すると、pFeatureLevelでサポートされている最高の機能レベルが返される
		NULL,									//このデバイスでサポートされている機能レベルの配列にある最初の要素を表すD3D_FEATURE_LEVELへのポインターを返す
		&pDeviceContext);				//デバイス コンテキストを表すID3D11DeviceContextオブジェクトへのポインターのアドレスを返す
	if (FAILED(hr))
	{
		MessageBox(NULL, "スワップチェーンの作成に失敗", "Error", MB_OK);
		return false;
	}
	return true;
}
bool DirectX11::InitDepthBuffer(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthBuf;
	HRESULT hr;
	SecureZeroMemory(&depthBuf, sizeof(depthBuf));

	//デプスバッファ(Zバッファ)の設定
	depthBuf.Width = width;											//幅
	depthBuf.Height = height;										//高さ
	depthBuf.MipLevels = 1;											//ミップマップ(テクスチャの画像を補完する)レベルの最大数
	depthBuf.ArraySize = 1;											//テクスチャー配列内のテクスチャーの数
	depthBuf.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//テキスチャーフォーマット
	depthBuf.SampleDesc.Count = 1;								//マルチサンプリング(アンチエイリアシング)パラメーターを指定する構造体
	depthBuf.SampleDesc.Quality = 0;							//マルチサンプリング(アンチエイリアシング)パラメーターを指定する構造体
	depthBuf.Usage = D3D11_USAGE_DEFAULT;					//テクスチャーの読み込みおよび書き込み方法を識別する値
	depthBuf.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//パイプラインステージへのバインドに関するフラグ
	depthBuf.CPUAccessFlags = 0;									//許可するCPUアクセスの種類を指定するフラグ
	depthBuf.MiscFlags = 0;											//他の一般性の低いリソース オプションを識別するフラグ

																	//深度バッファの作成
	hr = pDevice->CreateTexture2D(
		&depthBuf,
		NULL,
		&pDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "深度バッファの作成に失敗", "Error", MB_OK);
		return false;
	}

	return true;
}
bool DirectX11::InitDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//デスクリプションの設定
	depthStencilDesc.DepthEnable = true;							//深度テストを使用可能にする
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//深度ステンシルバッファーの中で、深度データによる変更が可能な部分を識別
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;				//深度データを既存の深度データと比較する関数

	depthStencilDesc.StencilEnable = true;							//ステンシルテストを使用可能にします。
	depthStencilDesc.StencilReadMask = 0xFF;						//深度ステンシルバッファーの中で、ステンシルデータを読み取る部分を識別
	depthStencilDesc.StencilWriteMask = 0xFF;						//深度ステンシルバッファーの中で、ステンシルデータを書き込む部分を識別

																	//法線がカメラの方向を向いているサーフェス(表面)を持つピクセルの深度テストとステンシルテストの結果を使用する方法を識別
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//法線がカメラと逆方向を向いているサーフェスを持つピクセルの深度テストとステンシル テストの結果を使用する方法を識別
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//深度ステンシルステートの作成
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "深度ステンシルステートの作成に失敗", "Error", MB_OK);
		return false;
	}

	//コンテキストに深度ステンシルステートを設定
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	return true;
}
bool DirectX11::InitStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//深度ステンシル ビューからアクセス可能なテクスチャーのサブリソースを指定する
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		//リソースのデータフォーマット
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//リソースのタイプ
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//デプスステンシルビューの作成
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "深度ステンシルビューの作成に失敗", "Error", MB_OK);
		return false;
	}

	return true;
}
bool DirectX11::InitRasterizerState()
{
	//ラスタライザーステートは、位置、カラー、テクスチャーなどの3Dデータを2Dサーフェイス上にレンダリングする方法を決定する	
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT hr;

	rasterDesc.AntialiasedLineEnable = false;	//線のアンチエイリアシングを有効。線を描画中でMultisampleEnableがfalseの場合にのみ適用
	rasterDesc.CullMode = D3D11_CULL_BACK;		//指定の方向を向いている三角形が描画されないことを示す
	rasterDesc.DepthBias = 0;					//指定のピクセルに加算する深度値
	rasterDesc.DepthBiasClamp = 0.0f;			//ピクセルの最大深度バイアス(偏り)
	rasterDesc.DepthClipEnable = true;			//距離に基づいてクリッピングを有効
	rasterDesc.FillMode = D3D11_FILL_SOLID;		//レンダリング時に使用する描画モードを決定
	rasterDesc.FrontCounterClockwise = false;	//三角形が前向きか後ろ向きかを決定。のパラメーターがtrueの場合、三角形の頂点がレンダーターゲット上で左回りならば三角形は前向きと見なされ、右回りならば後ろ向きと見なされ、このパラメーターがfalseの場合は逆になる
	rasterDesc.MultisampleEnable = false;		//マルチサンプリングのアンチエイリアシングを有効
	rasterDesc.ScissorEnable = false;			//シザー矩形カリングを有効
	rasterDesc.SlopeScaledDepthBias = 0.0f;		//指定のピクセルのスロープに対するスカラ

	hr = pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザーステートの作成に失敗", "Error", MB_OK);
		return false;
	}

	pDeviceContext->RSSetState(pRasterState);

	return true;
}
void DirectX11::InitViewport(int width, int height)
{
	D3D11_VIEWPORT viewport;

	//ビューポートの設定
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;


	pDeviceContext->RSSetViewports(1, &viewport);
}
bool DirectX11::InitAlphaBlending()
{
	D3D11_BLEND_DESC blendStateDesc;
	HRESULT hr;
	SecureZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	//アルファブレンディングの設定
	blendStateDesc.RenderTarget[0].BlendEnable = true;							//ブレンディングを有効(または無効) 
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//最初のRGBデータソースを指定
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//2番目のRGBデータソースを指定
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//RGBデータソースの組合せ方法を定義
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//最初のアルファ データ ソースを指定
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//2番目のアルファデータソースを指定
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//アルファ データ ソースの組合せ方法を定義
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;				//書き込みマスク

																				//有効なアルファブレンド状態を作成する
	hr = pDevice->CreateBlendState(&blendStateDesc, &pAlphaEnableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	//無効にしたアルファブレンドの状態を作成するように変更する
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	//無効なアルファブレンドの作成
	hr = pDevice->CreateBlendState(&blendStateDesc, &pAlphaDisableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
bool DirectX11::InitZBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));


	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthDisabledStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}