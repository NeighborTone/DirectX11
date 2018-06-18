#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ShowConsole();
	//�Q�[���G���W������
	Engine ge("DirectX11", 640, 480, true);

	//�J��������
	Camera camera3D;
	camera3D.pos = Vec3(0.0f, 0.0f, 0.0f);
	camera3D.angle.x = 20;
	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);

	Particle ef("Resource/fire.efk");
	Particle ef2;
	ef2.Load("Resource/testEf.efk");

	SoundSource sound, sound2;
	sound.Load("Resource/Grass.wav",true);
	sound2.Load("Resource/se.wav", true);
	EffectParameters::Equalizer_DESC eq;
	eq.FrequencyCenter3 = 200.0f;
	eq.Gain3 = 0.126f;
	EffectParameters::Reverb_DESC rev;
	rev.RoomSize = 50.0f;
	EffectParameters::Delay_DESC delay;
	delay.DelayTime = 300;
	EffectParameters::Limiter_DESC limiter;
	limiter.Loudness = 1100;
	sound2.SetMultiEffecter(eq, rev, delay, limiter);
	sound.PlayBGM(255,1.0f);


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
		if (KeyBoard::Down(KeyBoard::Key::KEY_X))
		{
			sound2.PlaySE();
			
			ef2.Play(Vec3(0, 0, 0));
		}
		sound.UpDate3DSound(Vec3(0, 0, 20), Vec3(camera3D.pos));
		sound2.UpDate3DSound(Vec3(0, 0, 0),Vec3(camera3D.pos));
		ef2.Draw(camera3D);



		//================================//
		//==========2DRendering==============//
		//================================//
		camera2D.Run(false);

		std::cout << sound2.GetCurrentSampleTime()<< std::endl;
	}

	//�I��
	return 0;

}