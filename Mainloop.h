#pragma once
#include "System.h"
#include "Engine.h"
#include <memory>
//メインループ処理を行う
class Mainloop final
{
private:
	MSG msg;
	System window;
	Engine* engine;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};