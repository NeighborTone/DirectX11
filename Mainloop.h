#pragma once
#include "System.h"

//���C�����[�v�������s��
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