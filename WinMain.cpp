#include "Engine.h"
#include "Model.h"
//仮----------------------------------------------------
bool HitBall(Vec3 &c1, Vec3& c2, float r,float r2)
{
	if ((c1.x - c2.x) * (c1.x - c2.x) + ((c1.y - c2.y) * (c1.y - c2.y)) + ((c1.z - c2.z) * (c1.z - c2.z)) <= (r + r) * (r2+ r2))
	{
		return true;
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ci_ext::Console();
	ShowConsole();
	constexpr int MAX = 5;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);

	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 7.0f, -20.0f);
	camera.SetPerspective(45.0f, 1, 10000.0f);
	//camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);

	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");
	Texture texture4("p.png");

	Mesh box[MAX];
	for (int i = 0; i < MAX; ++i)
	{
		box[i].CreateCube();
		box[i].GetMaterial().SetTexture(0, &texture2);
	}
	Mesh ground;
	ground.CreateCube();
	ground.GetMaterial().SetTexture(0, &texture4);
	ground.scale = 10;
	ground.scale.y = 1;

	PhysicsWorld physicsWorld;
	for (int i = 0; i < MAX; ++i)
	{
		physicsWorld.AddDynamicBox(Vec3(1, 1, 1), 5);
	}
	physicsWorld.AddStaticBox(Vec3(10, 1, 10));
	physicsWorld.pDynamicBox[0]->SetPosition(Vec3(-5.5f, 10, 0));
	physicsWorld.pDynamicBox[1]->SetPosition(Vec3(5.5f, 10, 0));
	physicsWorld.pDynamicBox[2]->SetPosition(Vec3(0, 10, 0));
	physicsWorld.pDynamicBox[3]->SetPosition(Vec3(0.5f, 11, 0));
	physicsWorld.pDynamicBox[4]->SetPosition(Vec3(-0.7f, 11, 0));

	while (ge.Run())
	{
		camera.Run();
		
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			Pad::Down(Pad::Button::PAD_START))
		{
			break;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
		{
			camera.pos.z += 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
		{
			camera.pos.z -= 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RIGHT))
		{
			camera.pos.x += 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera.pos.x -= 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RSHIFT))
		{
			camera.angles.y += 1.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LSHIFT))
		{
			camera.angles.y -= 1.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LCONTROL))
		{
			camera.pos.y -= 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RCONTROL))
		{
			camera.pos.y += 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_SPACE))
		{
			camera.angles.x += 0.5f;
		}
		static bool go = false;
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			go = true;
		}
		if (go)
		{
			physicsWorld.UpDate();
		}
		
		for (int i = 0; i < MAX; ++i)
		{
			box[i].pos = physicsWorld.pDynamicBox[i]->GetPosition();
			box[i].Draw();
		}

		ground.Draw();

		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}