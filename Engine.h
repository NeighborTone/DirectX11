#pragma once
#include "Graphics.h"

class Engine final
{
private:
	Engine();

	void Update();
	void Draw();

	Graphics* pGraphics;
	static Engine* instance;

public:
	
	~Engine();

	bool CreateGraphics(HWND hwnd);
	bool Create(HINSTANCE hinstance, HWND hwnd);
	void Run();
	void Release();
	Graphics* GetGraphics();
	static Engine* GetInst();

};

