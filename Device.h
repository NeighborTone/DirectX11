#pragma once
#pragma warning( disable : 4005 )	//DirectXマクロ再定義
#define RELEASE(p) {p->Release(); p = nullptr;}

#include "System.h"
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "Texture.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
namespace DX11
{
	class Direct3D
	{
	private:
		Direct3D();
		
		ID3D11Device*					pDevice;
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
		ID3D11BlendState*			pBlendState;
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
			D3DXVECTOR2 vTex; //テクスチャー座標
		};
		

		static Direct3D* GetInstace()
		{
			static Direct3D inst;
			return &inst;
		}
		bool MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);
		bool Create(System& win);
		void BeginDraw();
		void Flip();
		void DrawPrimitive();
		
		ID3D11Device* GetDevice()
		{
			return pDevice;
		}

		ID3D11DeviceContext* GetContext()
		{
			return pDeviceContext;
		}


		~Direct3D();
	};
}


