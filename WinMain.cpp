#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//�Q�[���G���W������
	Engine ge("hogehoge",640,480,true);
	while (!ge.Run())
	{

	}
	//�I��
	return 0;

}