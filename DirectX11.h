#pragma once
#pragma warning( disable : 4005 )	
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dCompiler.lib")
class DirectX11
{
public:
	DirectX11();
	~DirectX11();

	//DirectX11の初期化
	bool Create(int width, int height, bool vsync, HWND hwnd, bool isfullscreen);
	//描画開始
	void BeginScene(float r, float g, float b, float a);							
	//画面更新
	void EndScene();
	//アルファブレンドを切り替える
	void EnableAlphaBlending(bool enable);
	//Zバッファの切り替え
	void EnableZBuffer(bool enable);
	//デバイスを得る
	ID3D11Device* GetDevice();
	//コンテキストを得る
	ID3D11DeviceContext* GetDeviceContext();
private:
	//スワップチェインとデバイスの初期化
	bool InitSwapChain(HWND hwnd, bool isfullscreen, int width, int height, unsigned  int numerator, unsigned int denominator);
	//深度バッファ
	bool InitDepthBuffer(int width, int height);
	//ステンシルバッファ
	bool InitDepthStencilBuffer();
	//ステンシルビュー
	bool InitStencilView();
	//ラスタライザー
	bool InitRasterizerState();
	//ビューポート
	void InitViewport(int width, int height);
	//アルファブレンディング
	bool InitAlphaBlending();
	//Zバッファ
	bool InitZBuffer();

	bool vsyncEnabled;
	int videoCardMemory;
	char videoCardDescription[128];

	IDXGISwapChain*				pSwapChain;
	ID3D11Device*					pDevice;
	ID3D11DeviceContext*		pDeviceContext;
	ID3D11RenderTargetView*	pRenderTargetView;
	ID3D11Texture2D*				pDepthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilView*	pDepthStencilView;
	ID3D11RasterizerState*	pRasterState;
	ID3D11BlendState*			pAlphaEnableBlendingState;
	ID3D11BlendState*			pAlphaDisableBlendingState;
	ID3D11DepthStencilState* pDepthDisabledStencilState;
};

