#pragma once
#include "System.h"
#include "Graphic.h"
//���C�����[�v�������s��
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