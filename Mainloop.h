#pragma once
#include "System.h"
#include "DirectX.h"
//���C�����[�v�������s��
class Mainloop final
{
private:
	MSG msg;
	System window;
	DX11::DirectX directx;

public:
	Mainloop() {};
	~Mainloop() {};
	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Run();
};