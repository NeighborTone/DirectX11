#include "Engine.h"
#include "Camera.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//ゲームエンジン生成
	Engine ge("hogehoge",640,480,true);
	Camera camera;
	camera.pos = Vec3(0.0f, 1.0f, -5.0f);
	camera.SetPerspective(60.0f, 0.1f, 100.0f);
	camera.SetDepthTest(true);

	while (ge.Run())
	{
		camera.Update();
	}

	//終了
	return 0;

}