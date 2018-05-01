#include "Device.h"

namespace DX11
{
	Device::Device()
	{
		pDevice = nullptr;
		pDeviceContext = nullptr;
		pSwapChain = nullptr;
		pRTV = nullptr;
		pDS = nullptr;
		pDSV = nullptr;
	}

	Device::~Device()
	{
		if (pDevice != nullptr)
		{
			RELEASE(pDevice);
		}
		if (pDeviceContext != nullptr)
		{
			RELEASE(pDeviceContext);
		}
		if (pSwapChain != nullptr)
		{
			RELEASE(pSwapChain);

		}
		if (pRTV != nullptr)
		{
			RELEASE(pRTV);

		}
		if (pDS != nullptr)
		{
			RELEASE(pDS);

		}
		if (pDSV != nullptr)
		{
			RELEASE(pDSV);

		}
	}

	bool Device::InitDirect3D(System& win)
	{
		//デバイスとスワップチェーンの作成
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = 640;
		sd.BufferDesc.Height = 480;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = win.GetWindow();
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE, 
			NULL,
			0,
			&pFeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			pFeatureLevel,
			&pDeviceContext);

		//バックバッファーのレンダーターゲットビュー(RTV)を作成
		ID3D11Texture2D *pBack;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);

		pDevice->CreateRenderTargetView(pBack, NULL, &pRTV);
		pBack->Release();

		//デプスステンシルビュー(DSV)を作成
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = 640;
		descDepth.Height = 480;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		pDevice->CreateTexture2D(&descDepth, NULL, &pDS);

		pDevice->CreateDepthStencilView(pDS, NULL, &pDSV);

		//ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = 640;
		vp.Height = 480;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1, &vp);

		//レンダーターゲットビューとデプスステンシルビューをセット
		pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);

		return true;
	}

	void Device::Draw()
	{
		//画面クリア
		float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　RGBAの順
		pDeviceContext->ClearRenderTargetView(pRTV, ClearColor);//カラーバッファクリア
		pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア

		pSwapChain->Present(0, 0);//画面更新
	}
}
