#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>


/*! @class System
*   @brief ウィンドウを生成します
*/
class System final
{
private:
	int width;
	int height;
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str, int width,int height);
public:

	/*!
	* @brief コンストラクタです。
	* @param (title) ウィンドウタイトル
	* @param (width) ウィンドウ幅
	* @param (height) ウィンドウ高さ
	*/
	System(std::string title, int width, int height);
	System(const System&) = delete;
	~System();

	/*!
	* @brief ウィンドウ更新
	* @return 問題がなければtrue
	*/
	bool Run();

	/*!
	* @brief ウィンドウハンドルを取得します
	* @return HWND
	*/
	HWND GetHwnd();
	/*!
	* @brief ウィンドウの幅を取得します
	* @return int
	*/
	int GetWidth();
	/*!
	* @brief ウィンドウの高さを取得します
	* @return int
	*/
	int GetHeight();
};