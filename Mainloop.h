#pragma once
#include "System.h"
#include "Device.h"
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	DX11::Device directx;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};