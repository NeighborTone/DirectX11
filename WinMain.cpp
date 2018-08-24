#include "Engine/Engine.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Particle.h"
#include "Engine/Utilitys/Console.hpp"
#include "Engine/Utilitys/Easing.hpp"
#include "Engine/Sounds/SoundSource.h"
#include "Engine/Utilitys/Counter.hpp"
#include "Engine/Utilitys/Randam.hpp"
#include "Engine/Graphics/SpriteRender.h"
#include "Engine/Graphics/Light.h"

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
void GradationColor(Float4& color, Float4& colorDelta)
{
	color.r += colorDelta.r;
	color.g += colorDelta.g;
	color.b += colorDelta.b;
	if (color.r <= 0.2f || color.r >= 0.99f) colorDelta.r *= -1;
	if (color.b <= 0.2f || color.b >= 0.99f) colorDelta.b *= -1;
	if (color.g <= 0.2f || color.g >= 0.99f) colorDelta.g *= -1;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ShowConsole();
	//�Q�[���G���W������
	Engine ge("DirectX11", 640, 450, true);
	
	//�J��������
	Camera camera3D;
	camera3D.pos = Vec3(0,1,-5);
	camera3D.angle.x = 0;

	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);
	Texture t;
	t.Load("Resource/brick.jpg");
	Mesh m;
	m.GetMaterial().Load("Resource/Shader/mesh_l.hlsl");
	m.GetMaterial().SetTexture(0,&t);
	m.CreateSphere();
	Particle efs;
	efs.AddEffect("test","Resource/TestParticle.efk");
	efs.AddEffect("test2", "Resource/TestParticle2.efk");

	Counter cnt(0, 1, 0, 60);
	Mesh b;
	b.CreateCube();
	b.GetMaterial().Load("Resource/Shader/mesh_l.hlsl");
	b.GetMaterial().SetTexture(0, &t);
	b.scale = 10;
	b.scale.y = 0.5f;
	Sprite z("Resource/box.jpg");
	z.GetMaterial().Load("Resource/Shader/mesh_l.hlsl");
	Light directionalLight;
	directionalLight.type = Light::Type::Directional;
	directionalLight.angles = Vec3(-50.0f, 30.0f, 0.0);
	directionalLight.intensity = 0.01f;
	directionalLight.Draw();
	Light light;
	light.type = Light::Type::Point;
	light.pos.z = -1.2f;
	light.range = 20;
	Mouse::SetMousePos(0, 0);
	Float4 color(0.9f, 0.9f, 0.9f, 1);
	Float4 del(0.002f, 0.005f, 0.009f, 1);
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
		if (KeyBoard::Down(KeyBoard::Key::KEY_Z))
			efs.Play("test", Vec3(0, 0, 10));
		if (KeyBoard::Down(KeyBoard::Key::KEY_X))
			efs.Play("test2", Vec3(0, 0, 10));
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
			camera3D.pos.z += 0.5f;
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
			camera3D.pos.z -= 0.5f;

		efs.UpDate(Camera(camera3D));

		m.pos.y = 2;
		m.Draw();
		b.Draw();
		z.scale = 0.01f;
		z.pos.y = 5;
		z.Draw();
		GradationColor(color,del);
		light.color = color;
		light.pos = b.pos;
		light.pos.z = -1;
		light.pos.y = 2;
		light.Draw();
	
		//===================================//
		//==========2DRendering==============//
		//===================================//
		camera2D.Run(false);
		
		
	}

	//�I��
	return 0;

}