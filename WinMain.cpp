#include "Mainloop.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	
	Mainloop main;
	if (main.Init(hInstance, nCmdShow))
	{
		main.Run();
	}


	//�I��
	return 0;

}