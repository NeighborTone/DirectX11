#include "Engine/Engine.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Particle.h"
#include "Engine/Utilitys/Console.hpp"
#include "Engine/Utilitys/Easing.hpp"
#include "Engine/Sounds/SoundSource.h"
#include "Engine/Utilitys/Counter.hpp"
#include "Engine/Utilitys/Randam.hpp"
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
	Engine ge("DirectX11", 640, 450, true);
	
	//�J��������
	Camera camera3D;
	camera3D.pos.z = -20;
	camera3D.pos.y = 5;
	camera3D.angle.x = 0;

	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);
	

	Counter cnt(0, 1, 0, 60);

	Mouse::SetMousePos(0, 0);
	while (ge.Run())
	{
		//===================================//
		//==========3DRendering=================//
		//===================================//
		camera3D.Run(true);
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			DInput::Down(DInput::Button::PAD_START))
		{
			break;
		}
	

	
		camera3D.angle.x += Mouse::GetMousePosCenter().y * 0.3f;
		camera3D.angle.y += Mouse::GetMousePosCenter().x * 0.3f;
		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);

		Mouse::SetMousePos(0, 0); 
		Mouse::DrawCursor(false);
	}

	//�I��
	return 0;

}