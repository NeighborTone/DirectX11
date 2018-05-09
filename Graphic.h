#pragma once
#include "DirectX11.h"

class Graphic
{
public:
	Graphic(HWND hwnd);
	~Graphic();

	DirectX11 directx;
	DirectX11* GetDX();
	void Begine();
	void End();
	void Run();
private:
	HWND hwnd;
	void Update();
	
};