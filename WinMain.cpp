#include "Engine.h"
#include "Camera.h"
#include "Mesh.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//ゲームエンジン生成
	Engine ge("Directx11",640,480,true);
	Camera camera;
	camera.pos = Vec3(0.0f, 0.0f, -5.0f);
	//camera.SetPerspective(100.0f, 1, 100.0f);
	camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);
	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");

	Mesh mesh;
	mesh.CreateCube();
	mesh.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	mesh.GetMaterial().SetTexture(0, &texture1);
	mesh.scale *= 100;

	Mesh mesh2;
	mesh2.CreateCube();
	mesh2.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	mesh2.GetMaterial().SetTexture(0, &texture2);
	mesh2.scale *= 100;

	Mesh mesh3;
	mesh3.CreateCube();
	mesh3.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	mesh3.GetMaterial().SetTexture(0, &texture3);
	mesh3.scale *= 100;
	float x = 0;
	while (ge.Run())
	{
		camera.Update();
		camera.pos.x += 1;

		mesh.angle.y += 1.0f;
		mesh.pos.x = x;
		mesh.Draw();
		x += 1;
	
		mesh2.angle.x += 1.0f;
		mesh2.pos.x = 200.0f;
		mesh2.Draw();

		mesh3.angle.x -= 1.0f;
		mesh3.pos.x = -200.0f;
		mesh3.Draw();
	}

	//終了
	return 0;

}