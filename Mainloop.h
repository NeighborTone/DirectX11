#pragma once
#include "System.h"
#include "Device.h"
#include "Texture.h"
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	DX11::Device* DX_device;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};