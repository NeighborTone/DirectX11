#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	Console();
	ShowConsole();
	constexpr int BOX_MAX = 3;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);

	//カメラ生成
	Camera camera3D;
	camera3D.pos = Vec3(0.0f, 12.0f, -20.0f);
	camera3D.angle.x = 20;
	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);
	
	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1,0.1f,1000.0f);

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
	me.angle = 0;
	me.pos.x = camera3D.pos.x;
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
	physicsWorld.pRigidBody[BOX_MAX]->SetRotation(Vec3(rigidBall.angle));

	physicsWorld.AddRigidBody(new DynamicCylinder(cylinder.pos, 55, CylinderDir::Y, 1, 2));
	physicsWorld.pRigidBody[BOX_MAX+1]->SetRotation(Vec3(cylinder.angle));


	physicsWorld.AddGeometry(new StaticBox(ground.pos, ground.scale));
	physicsWorld.AddGeometry(new StaticBox(me.pos, me.scale));
	physicsWorld.AddGeometry(new StaticSphere(geomBall.pos, 1));

	physicsWorld.pGeometry[1]->SetPosition(me.pos);
	physicsWorld.pGeometry[1]->SetRotation(me.angle);

	Text text("Hoge");
	text.scale /= (float)text.GetSize().y;
	text.scale *= 2;
	SoundSource sound,sound2;
	sound.Load("Resource/Grass.wav");
	sound2.Load("Resource/se.wav");
	Engine::GetSoundSystem().AddSource(sound);
	Engine::GetSoundSystem().AddSource(sound2);
	EffectParameters::Equalizer_DESC eq;
	eq.FrequencyCenter3 = 200.0f;
	eq.Gain3 = 0.126f;
	EffectParameters::Reverb_DESC rev;
	rev.RoomSize = 50.0f;
	EffectParameters::Delay_DESC delay;
	delay.DelayTime = 300;
	EffectParameters::Limiter_DESC limiter;
	limiter.Loudness = 1100;
	sound2.SetMultiEffecter(eq,rev,delay,limiter);
	sound.PlayBGM();
	Easing ease;

	Sprite sp("Resource/カーソル.png");

	while (ge.Run())
	{
		//================================//
		//==========3DRendering==============//
		//================================//
		camera3D.Run(true);
		
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			Pad::Down(Pad::Button::PAD_START))
		{
			break;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
		{
			camera3D.pos.z += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
		{
			camera3D.pos.z -= 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RIGHT))
		{
			camera3D.pos.x += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera3D.pos.x -= 0.6f;
		}
		static bool go = false;
		static float speed = 0.06f;
		static int dir = 1;
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			go = true;
			//text.pos.y = ease.bounce.InOut(ease.Time(14), 20, 10);
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
			text.pos.y = ease.quad.In(ease.Time(10), 0, 10);
		}
		if (KeyBoard::Down(KeyBoard::Key::KEY_Z))
		{
			ef.Play(Vec3(0, 2, 3));
		}
		if (KeyBoard::Down(KeyBoard::Key::KEY_X))
		{
			sound2.PlaySE();
			ef2.Play(Vec3(0, 2, 3));
		}
		static int a = 1;
		if (KeyBoard::Down(KeyBoard::Key::KEY_V))
		{
			physicsWorld.AddRigidBody(new DynamicBox(Vec3(camera3D.pos.x, 2, camera3D.pos.z +10), Vec3(1, 1, 1), 5));
			
			
			++a;
		}
		physicsWorld.pRigidBody[BOX_MAX + a]->AddVelocity(Vec3(0, 0, 10));
		me.pos = physicsWorld.pGeometry[1]->GetPosition();
		me.Draw(physicsWorld.pGeometry[1]->GetRotation());

		
		ground.pos = physicsWorld.pGeometry[0]->GetPosition();
		ground.Draw();
		for (UINT i = 0; i < physicsWorld.pRigidBody.size(); ++i)
		{
			physicsWorld.pRigidBody[i]->Draw(texture2);
		}

		texture2.Attach(0);
		rigidBall.Draw(physicsWorld.pRigidBody[BOX_MAX]->GetRotation(),true);
		rigidBall.pos = physicsWorld.pRigidBody[BOX_MAX]->GetPosition();

		texture3.Attach(0);
		geomBall.pos = physicsWorld.pGeometry[2]->GetPosition();
		geomBall.Draw(true);

		texture5.Attach(0);
		cylinder.pos = physicsWorld.pRigidBody[BOX_MAX+1]->GetPosition();
		cylinder.Draw(physicsWorld.pRigidBody[BOX_MAX+1]->GetRotation(),true);

		ef.Draw(camera3D);
		ef2.Draw(camera3D);

		text.Draw();

		//================================//
		//==========2DRendering==============//
		//================================//
		camera2D.Run(false);
		++sp.angle.z;
	
		sp.Draw();












		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}