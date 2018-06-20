#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ShowConsole();
	//ゲームエンジン生成
	Engine ge("DirectX11", 640, 480, true);

	//カメラ生成
	Camera camera3D;
	camera3D.pos.z = -200;
	camera3D.pos.y = 50;
	camera3D.angle.x = 20;
	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);

	Texture tex("Resource/box.jpg");
	Texture tex2("Resource/white.png");
	PhysicsWorld physicsWorld;
	struct Wall
	{
		Mesh mesh;
		int ID;
	};
	Wall box[3];
	Wall me;
	me.mesh.GetMaterial().SetTexture(0,&tex2);
	me.mesh.CreateCube();
	me.mesh.pos = camera3D.pos;
	me.mesh.pos.y -= 5;
	me.mesh.pos.z += 10;
	for (int i = 0; i < 3; ++i)
	{
		box[i].mesh.GetMaterial().SetTexture(0, &tex);
		box[i].mesh.CreateCube();
	}
	//L
	box[0].mesh.pos.x = -25;
	box[0].mesh.pos.y = 10.f;
	box[0].mesh.pos.z = 0;
	box[0].mesh.scale.y = 20;
	box[0].mesh.scale.z = 500;
	//R
	box[1].mesh.pos.x = 25;
	box[1].mesh.pos.y = 10.f;
	box[1].mesh.pos.z = 0;
	box[1].mesh.scale.y = 20;
	box[1].mesh.scale.z = 500;
	//C
	box[2].mesh.pos.x = 0;
	box[2].mesh.pos.z = 0;
	box[2].mesh.angle.x = 0;
	box[2].mesh.scale.x = 50;
	box[2].mesh.scale.y = 1;
	box[2].mesh.scale.z = 500;

	for (int i = 0; i < 3; ++i)
	{
		box[i].ID = physicsWorld.AddGeometry(new StaticBox(box[i].mesh.pos, box[i].mesh.scale));
		physicsWorld.pGeometry[box[i].ID]->SetRotation(box[i].mesh.angle);
	}
	me.ID = physicsWorld.AddGeometry(new StaticBox(me.mesh.pos, me.mesh.scale));
	physicsWorld.pGeometry[me.ID]->SetRotation(me.mesh.angle);


	while (ge.Run())
	{
		physicsWorld.UpDate();
		//===================================//
		//==========3DRendering=================//
		//===================================//
		camera3D.Run(true);
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			Pad::Down(Pad::Button::PAD_START))
		{
			break;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RIGHT))
		{
			camera3D.pos.x += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera3D.pos.x -= 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LCONTROL))
		{
			camera3D.pos.y += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RCONTROL))
		{
			camera3D.pos.y -= 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
		{
			camera3D.pos.z += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
		{
			camera3D.pos.z -= 0.6f;
		}

		for (int i = 0; i < 3; ++i)
		{
			box[i].mesh.pos = physicsWorld.pGeometry[box[i].ID]->GetPosition();
			box[i].mesh.Draw();
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			me.mesh.pos.y -= 1;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_W))
		{
			me.mesh.pos.y += 1;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_A))
		{
			me.mesh.pos.x -= 1;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_D))
		{
			me.mesh.pos.x += 1;
		}
		physicsWorld.pGeometry[me.ID]->SetPosition(me.mesh.pos);
		me.mesh.pos = physicsWorld.pGeometry[me.ID]->GetPosition();
		physicsWorld.IsHitGeom(box[2].ID, me.ID);
		me.mesh.Draw();

		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);

		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}