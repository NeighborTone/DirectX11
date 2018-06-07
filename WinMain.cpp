#include "Engine.h"
#include "Model.h"
#include "BasicShapes.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ci_ext::Console();
	ShowConsole();
	constexpr int MAX = 100;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);

	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 12.0f, -20.0f);
	camera.angles.x = 20;
	camera.SetPerspective(45.0f, 1, 10000.0f);
	//camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);

	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");
	Texture texture4("p.png");

	Model model("ball.fbx");

	model.pos.y = 8;
	model.angles.x = 0;

	Mesh me;
	me.CreateCube();
	me.GetMaterial().SetTexture(0, &texture1);
	me.scale = 1;
	me.scale.x = 10;
	me.pos.x = camera.pos.x;
	me.pos.y = 1;
	me.pos.z = -10;

	Mesh ground;
	ground.CreateCube();
	ground.GetMaterial().SetTexture(0, &texture4);
	ground.scale = 10;
	ground.scale.y = 1;


	PhysicsWorld physicsWorld;
	physicsWorld.AddDynamicSphere(model.pos,0.5f,55);
	physicsWorld.pDynamicSphere[0]->SetAngle(model.angles);
	physicsWorld.AddDynamicBox(Vec3(0,10,0),Vec3(1,1,1),5);

	physicsWorld.AddStaticBox(ground.scale);
	physicsWorld.AddStaticBox(me.scale);
	physicsWorld.pStaticBox[1]->SetPosition(me.pos);

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
			camera.pos.z += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
		{
			camera.pos.z -= 0.6f;
		}
		static bool go = false;
		static float speed = 0.06f;
		static int dir = 1;
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			go = true;

		}
		if (go)
		{
			physicsWorld.UpDate();
			physicsWorld.UpDate();
			if (dir == -1 && me.pos.z <= -8)
			{
				dir = 1;
			}
			if (dir == 1 && me.pos.z >= 8)
			{
				dir = -1;
			}
			me.pos.z += speed * dir;
			physicsWorld.pStaticBox[1]->SetPosition(me.pos);
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_Z))
		{

		}
		if (KeyBoard::On(KeyBoard::Key::KEY_X))
		{

		}
		physicsWorld.pStaticBox[0]->SetPosition(Vec3(0, 0, 0));
		me.pos = physicsWorld.pStaticBox[1]->GetPosition();
		me.Draw();
		model.pos = physicsWorld.pDynamicSphere[0]->GetPosition();
		ground.pos = physicsWorld.pStaticBox[0]->GetPosition();
		ground.Draw();
		physicsWorld.pDynamicBox[0]->Draw(texture2);
		std::cout << Engine::GetFps().GetFrameRate() << std::endl;

		texture2.Attach(0);
		model.Draw();
		model.angles = physicsWorld.pDynamicSphere[0]->GetAngle();

	}

	//終了
	return 0;

}