#include "FrameWork.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	FrameWork* framwork = new FrameWork;

	if (framwork->init())
	{
		framwork->Run();
	}
	delete framwork;
	return 0;
}