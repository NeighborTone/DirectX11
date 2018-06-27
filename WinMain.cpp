#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"
#include "XInput.h"
#include "SoundSource.h"
#include "Counter.hpp"
//TODO:
//:�����G���W���Ǘ��N���X�̎g�����肪�����̂ŉ��Ƃ�����
//:�_��ȏՓˌ��m
//:�v���~�e�B�u�̒ǉ�
//:�T�E���h�G�t�F�N�g�N���X���_��ɂ���
//:�x�����x��4�ł̌x�����ŏ����ɂ���
//:�p�b�h�A�}�E�X�A�L�[���͂̃O���[�o���ϐ��̉���
//:���f���ɕ����̃e�N�X�`�����A�^�b�`�ł���悤�ɂ���
//:�X�J�C�{�b�N�X
//:���̑��̃��f���`���̓ǂݍ���
//:�X�v���C�g�A�j���[�V����
//:���C�g�̔z�u
//:�t���X�N���[���ŉ�ʂ��䂪��
//:�t�H�O
//:�̂ɍ�����@�\�̃��t�@�N�^�����O
//:�����ĂȂ��Ƃ����Doxy�`���̃R�����g���ǉ�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ShowConsole();
	//�Q�[���G���W������
	Engine ge("DirectX11", 640, 480, true);
	
	//�J��������
	Camera camera3D;
	camera3D.pos.z = -20;
	camera3D.pos.y = 5;
	camera3D.angle.x = 20;
	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);
	
	Texture tex("Resource/brick.jpg");
	
	//Model m("Resource/ball.fbx");
	Easing ease;
	Counter cnt(0, 1, 0, 60);
	Particle effect;
	effect.Load("Resource/testParticle.efk");
	Text text("2DHogeText",20.0f,"���C���I");

	//text.scale = 1.0f/16.0f;
	while (ge.Run())
	{
	
		//===================================//
		//==========3DRendering=================//
		//===================================//
		camera3D.Run(true);
		XInput::UpDate();
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			XInput::Down(XInput::ButtonID::START,XInput::ID::P1)||
			XInput::Down(XInput::ButtonID::START, XInput::ID::P2))
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
		effect.Draw(camera3D);
		++cnt;
		if (cnt.GetCurrentCount() >= 60)
		{

			effect.Play();
		}
		//tex.Attach(0);
		//m.Draw();
		
		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);
		text.color = Float4(1,0,0,1);
		text.pos.y = ease.quad.Out(ease.Time(10),200, 10 - 200);
		text.Draw();

		text.color = Float4(0, 1, 0, 1);
		text.pos.y = -10;
		text.Draw();
		//std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//�I��
	return 0;

}