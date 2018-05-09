#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
//�E�B���h�E�֘A�̏������s��
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
	void SetWindowSize(UINT w = 640, UINT h = 480);				//��ʃT�C�Y�w��
	bool Create(std::string str , HINSTANCE& hInstance, int& nCmdShow);		//�E�B���h�E����
	UINT Width() { return SCREEN_W; }
	UINT Height() { return SCREEN_H; }
	HINSTANCE ins;
	HWND GetWindow();
};