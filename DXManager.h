#pragma once
#pragma warning( disable : 4005 )	//DirectXƒ}ƒNƒÄ’è‹`
#define RELEASE(p) {p->Release(); p = nullptr;}
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dCompiler.lib")
class DXManager
{
private:
	bool InitSwapChain(HWND hwnd, bool isfullscreen, int width, int height, unsigned  int numerator, unsigned int denominator);
	bool InitDepthBuffer(int width, int height);
	bool InitDepthStencilBuffer();
	bool InitStencilView();
	bool InitRasterizerState();
	void InitViewport(int width, int height);
	bool InitAlphaBlending();
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

public:
	DXManager();
	~DXManager();

	bool Init(int width, int height, bool vsync, HWND hwnd, bool isfullscreen);
	void BeginScene(float r, float g, float b, float a);
	void EndScene();
	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();


};

