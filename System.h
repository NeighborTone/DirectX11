#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
//�E�B���h�E�֘A�̏������s��
class System final
{
private:
	HWND handle;
public:
	System();
	System(const System&) = delete;
	~System();
	bool Create(std::string str , HINSTANCE& hInstance, int& nCmdShow);		//�E�B���h�E����
	HINSTANCE ins;
	HWND GetWindow();
};