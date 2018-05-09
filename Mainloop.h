#pragma once
#include "System.h"
#include "Graphic.h"
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	Graphic* dx;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};