#pragma once
#include "System.h"
#include "Direct3D.h"

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
	* @brief ウィンドウの幅を返します
	* @return int
	*/
	static int GetWindowWidth();
	/*!
	* @brief ウィンドウの高さを返します
	* @return int
	*/
	static int GetWindowHeight();
private:
	static std::string title;
	static int width;
	static int height;
	static bool isFullScreen;
	static System& GetWindow();
	static Direct3D& GetDirect3D();
};

