#pragma once
#include "System.h"
#include "DirectX11.h"
//���C�����[�v�������s��
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