#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ci_ext::Console();
	ShowConsole();
	constexpr int BOX_MAX = 100;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);

	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 12.0f, -20.0f);
	camera.angle.x = 20;
	camera.SetPerspective(45.0f, 1, 10000.0f);
	//camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);

	Texture texture1("Resource/box.jpg");
	Texture texture2("Resource/brick.jpg");
	Texture texture3("Resource/white.png");
	Texture texture4("Resource/p.png");
	Texture texture5("Resource/white.png");
	Model rigidBall("Resource/ball.fbx");

	rigidBall.pos.y = 8;
	rigidBall.angle.x = -90;

	Model geomBall("Resource/ball.fbx");
	geomBall.pos.y = 5;
	geomBall.pos.z = 1;

	Model cylinder("Resource/cylinder.fbx");
	cylinder.pos.y = 12;
	cylinder.pos.z = -1;
	cylinder.angle.x = -90;

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
	ground.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	ground.scale = 10;
	ground.scale.y = 1;

	Particle ef("Resource/fire.efk");
	Particle ef2;
	ef2.Load("Resource/testEf.efk");

	PhysicsWorld physicsWorld;

	for (int i = 0; i < BOX_MAX; ++i)
	{
		physicsWorld.AddRigidBody(new DynamicBox(Vec3(0, 0, 0), Vec3(1, 1, 1), 5));
		physicsWorld.pRigidBody[i]->SetPosition(Vec3(-5.0f + (float)i * 0.09f, 10 + (float)i * 3.5f, 0));
		physicsWorld.pRigidBody[i]->SetRotation(Vec3(45, 0, 40));
	}

	physicsWorld.AddRigidBody(new DynamicSphere(rigidBall.pos, 1, 55));
	physicsWorld.pRigidBody[100]->SetRotation(Vec3(rigidBall.angle));

	physicsWorld.AddRigidBody(new DynamicCylinder(cylinder.pos, 55, CylinderDir::Y, 1, 2));
	physicsWorld.pRigidBody[101]->SetRotation(Vec3(cylinder.angle));


	physicsWorld.AddGeometry(new StaticBox(ground.pos, ground.scale));
	physicsWorld.AddGeometry(new StaticBox(me.pos, me.scale));
	physicsWorld.AddGeometry(new StaticSphere(geomBall.pos, 1));

	physicsWorld.pGeometry[1]->SetPosition(me.pos);

	Text text("Hoge",25);
	text.scale /= (float)text.GetSize().y;
	text.pos.y = 10;
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
		if (KeyBoard::On(KeyBoard::Key::KEY_RIGHT))
		{
			camera.pos.x += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera.pos.x -= 0.6f;
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
			physicsWorld.pGeometry[1]->SetPosition(me.pos);
	
		}
		if (KeyBoard::Down(KeyBoard::Key::KEY_Z))
		{
			ef.Play(Vec3(0, 2, 3));
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_X))
		{
			ef2.Play(Vec3(0, 2, 3));
		}
		me.pos = physicsWorld.pGeometry[1]->GetPosition();
		me.Draw();

		
		ground.pos = physicsWorld.pGeometry[0]->GetPosition();
		ground.Draw();
		for (UINT i = 0; i < physicsWorld.pRigidBody.size(); ++i)
		{
			physicsWorld.pRigidBody[i]->Draw(texture2);
		}

		texture2.Attach(0);
		rigidBall.Draw(physicsWorld.pRigidBody[100]->GetRotation());
		rigidBall.pos = physicsWorld.pRigidBody[100]->GetPosition();

		texture3.Attach(0);
		geomBall.pos = physicsWorld.pGeometry[2]->GetPosition();
		geomBall.Draw();

		texture5.Attach(0);
		cylinder.pos = physicsWorld.pRigidBody[101]->GetPosition();
		cylinder.Draw(physicsWorld.pRigidBody[101]->GetRotation());

		ef.Draw(camera);
		ef2.Draw(camera);

		text.Draw();
		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}