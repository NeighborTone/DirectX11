#pragma once
#include "DirectX11.h"

class Graphic
{
public:
	Graphic(HWND hwnd);
	~Graphic();

	DirectX11 directx;
	void Run();
private:
	HWND hwnd;
	void Update();
	void Draw();
};