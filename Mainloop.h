#pragma once
#include "System.h"
#include "DirectX11.h"
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	DirectX11 dx;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};