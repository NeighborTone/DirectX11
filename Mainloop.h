#pragma once
#include "System.h"
#include "Engine.h"
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	 

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};