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


	Model model("Lightbulb.fbx");

	model.pos.y = 2;
	model.angles.x = -90;

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

	Mesh me;
	me.CreateCube();
	me.GetMaterial().SetTexture(0,&texture1);
	me.scale = 1;
	me.scale.x = 10;
	me.pos.x = camera.pos.x;
	me.pos.y = 1;
	me.pos.z = -10;
	PhysicsWorld physicsWorld;
	for (int i = 0; i < MAX; ++i)
	{
		physicsWorld.AddDynamicBox(box[i].pos, box[i].scale, 5);
	}

	physicsWorld.AddStaticBox(ground.scale);
	physicsWorld.AddStaticBox(me.scale);
	physicsWorld.pStaticBox[1]->SetPosition(me.pos);


	for (int i = 0; i < MAX; ++i)
	{
		physicsWorld.pDynamicBox[i]->SetPosition(Vec3(-5.0f + (float)i * 0.09f, 10 + (float)i * 3.5f, 0));
	}


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
			if (dir == -1 && me.pos.z <= -8)
			{
				dir = 1;
			}
			if(dir == 1 && me.pos.z >= 8)
			{
				dir = -1;
			}
			me.pos.z += speed * dir;
			physicsWorld.pStaticBox[1]->SetPosition(me.pos);
		}
		
		physicsWorld.pStaticBox[0]->SetPosition(Vec3(0, 0, 0));
	
		for (UINT i = 0; i < physicsWorld.pDynamicBox.size(); ++i)
		{
			box[i].pos = physicsWorld.pDynamicBox[i]->GetPosition();
			box[i].angle = physicsWorld.pDynamicBox[i]->GetAngle();
			box[i].Draw();
		}
		ground.pos = physicsWorld.pStaticBox[0]->GetPosition();
		ground.Draw();
		me.pos = physicsWorld.pStaticBox[1]->GetPosition();
		me.Draw();
		std::cout << Engine::GetFps().GetFrameRate() << std::endl;

		texture2.Attach(0);
		model.Draw();
	

	}

	//終了
	return 0;

}