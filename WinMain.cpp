#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//ゲームエンジン生成
	Engine ge;
	while (!ge.Run())
	{

	}
	//終了
	return 0;

}