#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "SystemDefs.hpp"

/*! @class System
*   @brief ウィンドウを生成します
*/
class System final
{
private:
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str);
public:

	/*!
	* @brief コンストラクタです。
	* @param (title) ウィンドウタイトル
	*/
	System(std::string title);
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
};