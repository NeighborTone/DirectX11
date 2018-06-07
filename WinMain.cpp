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

	Texture texture1("Resource/box.jpg");
	Texture texture2("Resource/brick.jpg");
	Texture texture3("Resource/white.png");
	Texture texture4("Resource/cursor.png");
	Texture texture5("Resource/white.png");
	Model rigidBall("Resource/ball.fbx");

	rigidBall.pos.y = 8;
	rigidBall.angles.x = 0;

	Model geomBall("Resource/ball.fbx");
	geomBall.pos.y = 5;
	geomBall.pos.z = 1;

	Model cylinder("Resource/cylinder.fbx");
	cylinder.pos.y = 12;
	cylinder.pos.z = -1;
	cylinder.angles.x = -90;

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
	physicsWorld.AddDynamicSphere(rigidBall.pos,1,55);
	physicsWorld.pDynamicSphere[0]->SetQuaternion(rigidBall.angles);

	for (int i = 0; i < MAX; ++i)
	{
		physicsWorld.AddDynamicBox(Vec3(0, 0, 0), Vec3(1, 1, 1), 5);
		physicsWorld.pDynamicBox[i]->SetPosition(Vec3(-5.0f + (float)i * 0.09f, 10 + (float)i * 3.5f, 0));
	}

	physicsWorld.AddDynamicCylinder(cylinder.pos,55,CylinderDir::Y,1,2);


	physicsWorld.AddStaticBox(ground.pos, ground.scale);
	physicsWorld.AddStaticBox(me.pos, me.scale);
	physicsWorld.AddStaticSphere(geomBall.pos,1);
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
		me.pos = physicsWorld.pStaticBox[1]->GetPosition();
		me.Draw();

		rigidBall.pos = physicsWorld.pDynamicSphere[0]->GetPosition();
		ground.pos = physicsWorld.pStaticBox[0]->GetPosition();
		ground.Draw();
		for (int i = 0; i < MAX; ++i)
		{
			physicsWorld.pDynamicBox[i]->Draw(texture2);
		}

		texture2.Attach(0);
		rigidBall.Draw();
		rigidBall.angles = physicsWorld.pDynamicSphere[0]->GetQuaternion();

		texture3.Attach(0);
		geomBall.pos = physicsWorld.pStaticSphere[0]->GetPosition();
		geomBall.Draw();

		texture5.Attach(0);
		cylinder.pos = physicsWorld.pDynamicCylinder[0]->GetPosition();
		cylinder.Draw();


		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}