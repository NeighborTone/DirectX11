#pragma once
#include "System.h"
#include "Engine.h"
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