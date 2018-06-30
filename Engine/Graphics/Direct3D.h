#pragma once
#pragma warning (disable : 4005)	//DirectX側の警告(マクロ二重定義)
#include "System.h"
#include <d2d1_1.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <wincodec.h>
#include <atlbase.h>
#include <wrl.h>


#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


/*! @class Direct3D
*   @brief DirectX11の初期化を行います
*/
class Direct3D : public System::Proceedable
{
public:

	/*!
	* @brief コンストラクタです。
	* @param (isFullScreen) フルスクリーンにするか(falseで全画面)
	*/
	Direct3D(bool isFullScreen);
	virtual ~Direct3D();
	ID3D11Device& GetDevice3D() const;
	ID3D11DeviceContext& GetContext3D() const;
	ID2D1Device& GetDevice2D() const;
	ID2D1DeviceContext& GetContext2D() const;
	IDXGISwapChain& GetSwapChain() const;
	IWICImagingFactory& GetTextureFactory() const;
	IDWriteFactory& GetTextFactory() const;

	/*!
	*   @brief 画面の更新処理を行います
	*/
	void UpDate();
	

private:
	//ComPtrマイクロソフトのシェアポインタ
	//CComPtrマイクロソフトのユニークポインタ

	bool Create();
	bool IsCreateDevice();
	bool IsCreateBlendState();
	bool IsCreateD2D();
	void CreateSwapChain();
	void SetViewport();
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override;
	static bool isFullScreen;
	//! DirectX11のデバイス
	ATL::CComPtr<ID3D11Device>			     device3D;
	//! DirectX11のデバイスコンテキスト
	ATL::CComPtr<ID3D11DeviceContext>      context3D;
	//! Direct2Dのデバイス
	ATL::CComPtr<ID2D1Device>				      device2D;
	//! Direct2Dのデバイスコンテキスト
	ATL::CComPtr<ID2D1DeviceContext>        context2D;
	//! Direct2D.3Dで描画するためのスワップチェーン(画面出力)
	ATL::CComPtr<IDXGISwapChain>			      swapChain;
	//! Direct2Dで画像を描画するための情報を保持する
	ATL::CComPtr<IWICImagingFactory>  textureFactory;
	//! フォントデーターなどの状態情報を保持する
	Microsoft::WRL::ComPtr<IDWriteFactory> textFactory;		
};
