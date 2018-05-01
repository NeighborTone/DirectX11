#pragma once
#pragma warning( disable : 4005 )	//DirectXƒ}ƒNƒÄ’è‹`
#define RELEASE(p) {p->Release(); p = nullptr;}
#include "System.h"
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
namespace DX11
{
	class Device
	{
	public:

		ID3D11Device * pDevice;
		ID3D11DeviceContext*		pDeviceContext;
		IDXGISwapChain*				pSwapChain;
		ID3D11RenderTargetView*  pRTV;
		ID3D11Texture2D*				pDS;
		ID3D11DepthStencilView*	pDSV;

		bool InitDirect3D(System& win);
		void Draw();
		Device();
		~Device();
	};
}


