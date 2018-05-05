#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
//ウィンドウ関連の処理を行う
class System final
{
private:
	HWND handle;
public:
	System();
	System(const System&) = delete;
	~System();
	bool Create(std::string str , HINSTANCE& hInstance, int& nCmdShow);		//ウィンドウ生成
	HINSTANCE ins;
	HWND GetWindow();
};