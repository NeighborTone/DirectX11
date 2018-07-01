#pragma once
#include "Inputs/Keyboard.h"
#include "Inputs/Mouse.h"
#include "Inputs/Pad.h"
#include "Inputs/Direct_Input.h"
#include "Inputs/XInput.h"
#include "Graphics/System.h"
#include "Graphics/Direct3D.h"
#include "Graphics/Camera.h"
#include "Graphics/Text.h"
#include "Utilitys/FPS.h"
#include "Physics/Physics.h"

/*! @class Engine
*   @brief 描画等の更新処理や各ハンドルをまとめます
*   @detail コンストラクタで生成されます。名前、幅、高さ、全画面にするか指定してください
*/
class Engine final
{
public:
	/*!
	* @brief コンストラクタです。
	* @param (title) ウィンドウタイトル
	* @param (width) ウィンドウ幅
	* @param (height) ウィンドウ高さ
	* @param (isFullScreen) フルスクリーンにするか(falseで全画面)
	*/
	Engine(std::string WindowTitle,int width,int height, bool isFullScreen);
	Engine() = delete;
	Engine(const Engine&) = delete;
	~Engine();
	/*!
	* @brief COMの初期化を行います。複数回実行されません
	*/
	static void COMInitialize();
	/*!
	* @brief 各機能の更新処理を行います
	* @return 問題がなければtrueを返します
	*/
	static bool Run();

	/*!
	* @brief ウィンドウハンドルを返します
	* @return HWND
	*/
	static HWND GetWindowHandle();
	/*!
	* @brief ウィンドウのクライアント領域を取得します
	* @return DirectX::XINT2
	*/
	static DirectX::XMINT2 GetWindowSize();
	/*!
	* @brief DirectX11のデバイスを取得します
	* @return ID3D11Device&
	*/
	static ID3D11Device& GetDXDevice3D();
	/*!
	* @brief DirectX11のデバイスコンテキストを取得します
	* @return ID3D11DeviceContext&
	*/
	static ID3D11DeviceContext& GetDXContext3D();
	/*!
	* @brief Direct2Dのデバイスを取得します
	* @return ID2D1Device&
	*/
	static ID2D1Device& GetDXDevice2D();
	/*!
	* @brief Direct2Dのデバイスコンテキストを取得します
	* @return ID2D1DeviceContext&
	*/
	static ID2D1DeviceContext& GetDXContext2D();
	/*!
	* @brief スワップチェインを取得します
	* @return IDXGISwapChain&
	*/
	static IDXGISwapChain& GetDXSwapChain();
	/*!
	* @brief テクスチャーファクトリーを取得します
	* @return IWICImagingFactory&
	*/
	static IWICImagingFactory& GetTextureFactory();
	/*!
	* @brief テキストファクトリーを取得します
	* @return IDWriteFactory&
	*/
	static IDWriteFactory& GetTextFactory();
	/*!
	* @brief FPSクラスを取得します
	* @return FPS&
	*/
	static FPS& GetFps();

	/*!
	* @brief Physicsクラスを取得します
	* @return Physics&
	*/
	static Physics& GetPhysics();

private:
	static std::string title;
	static int width;
	static int height;
	static bool isFullScreen;
	static System& GetWindow();
	static Direct3D& GetDirect3D();

};

