#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"
//TODO:
//:�����G���W���Ǘ��N���X�̎g�����肪�����̂ŉ��Ƃ�����
//:�_��ȏՓˌ��m
//:ogg�t�@�C���̍Đ�
//:�v���~�e�B�u�̒ǉ�
//:�T�E���h�G�t�F�N�g�N���X��
//:XInput�ɑΉ�
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
		Vec3 vel;
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
	me.ID = physicsWorld.AddRigidBody(new DynamicBox(me.mesh.pos, me.mesh.scale,10));
	physicsWorld.pRigidBody[me.ID]->SetRotation(me.mesh.angle);
	me.vel = 0;
	physicsWorld.SetGravity(Vec3_d(0, 0, 0));
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
		me.mesh.pos = physicsWorld.pRigidBody[me.ID]->GetPosition();
		me.mesh.Draw();
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			me.vel.y -= 8;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_W))
		{
			me.vel.y += 8;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_A))
		{
			me.vel.x -= 8;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_D))
		{
			me.vel.x += 8;
		}
		physicsWorld.pRigidBody[me.ID]->AddVelocity(me.vel);
		me.vel = 0;
		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);

		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//�I��
	return 0;

}