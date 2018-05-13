#pragma once
#include "System.h"
#include "Direct3D.h"

/*! @class Engine
*   @brief 描画等の更新処理や各ハンドルをまとめます
*/
class Engine final
{
public:
	Engine(std::string WindowTitle);
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
private:
	static std::string title;
	static System& GetWindow();
	static Direct3D& GetDirect3D();
};

