#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
//ウィンドウ関連の処理を行う
class System final
{
private:
	HWND handle;
	UINT SCREEN_W;
	UINT SCREEN_H;
public:
	System();
	System(const System&) = delete;
	~System();
	void SetWindowSize(UINT w = 640, UINT h = 480);				//画面サイズ指定
	bool Create(std::string str , HINSTANCE& hInstance, int& nCmdShow);		//ウィンドウ生成
	UINT Width() { return SCREEN_W; }
	UINT Height() { return SCREEN_H; }
	HINSTANCE ins;
	HWND GetWindow();
};