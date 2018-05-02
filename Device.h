#pragma once
#pragma warning( disable : 4005 )	//DirectXマクロ再定義
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
		//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体
		struct SIMPLESHADER_CONSTANT_BUFFER
		{
			D3DXMATRIX mWVP;
		};
		//頂点の構造体
		struct SimpleVertex
		{
			D3DXVECTOR3 Pos; //位置
		};
		ID3D11Device * pDevice;
		ID3D11DeviceContext*		pDeviceContext;
		IDXGISwapChain*				pSwapChain;
		ID3D11RenderTargetView*  pRTV;
		ID3D11Texture2D*				pDS;
		ID3D11DepthStencilView*	pDSV;
		ID3D11InputLayout*			pVertexLayout;
		ID3D11VertexShader*		pVertexShader;
		ID3D11PixelShader*			pPixelShader;
		ID3D11Buffer*					pConstantBuffer;
		ID3D11Buffer*					pVertexBuffer;

		bool MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);
		bool InitDirect3D(System& win);
		void Draw();
		Device();
		~Device();
	};
}


