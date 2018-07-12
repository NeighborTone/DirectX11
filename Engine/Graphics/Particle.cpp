#include "Particle.h"
#include "../Engine.h"
#include <iostream>
Particle::Particle()
{
	Create();
}

void Particle::Create()
{
	// �`��Ǘ��C���X�^���X�̐���
	renderer = EffekseerRendererDX11::Renderer::Create(
		&Engine::GetDXDevice3D(),
		&Engine::GetDXContext3D(),
		MAX_SPRITS);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	manager = Effekseer::Manager::Create(MAX_SPRITS);
	// ���W�n�̎w��(RH�ŉE��n�ALH�ō���n)
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	// �`����@���w�肵�܂��B�Ǝ��Ɋg�����邱�Ƃ��ł��܂��B
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// �e�N�X�`���摜�̓Ǎ����@�̎w��(�p�b�P�[�W������ǂݍ��ޏꍇ�g������K�v������܂��B)
	manager->SetTextureLoader(renderer->CreateTextureLoader());
}


void Particle::SetMatrix(Camera& camera)
{
	pCamera = &camera;
}

void Particle::AddEffect(const std::string name, const char * filePass)
{
	try
	{
		//���O�̏d���h�~
		if (effects.find(name) != effects.end())
		{
			throw name + "�̂͊��ɓo�^����Ă��܂�";
		}
	}
	catch(const std::string str)
	{
		std::cerr << str;
	}

	EFK_CHAR pass[64];
	Effekseer::ConvertUtf8ToUtf16((int16_t*)pass, 64, (const int8_t*)filePass);
	Effekseer::Effect* efk = Effekseer::Effect::Create(manager, pass);
	if (efk == nullptr) 
	{
		return;
	}
	//���X�g�֓o�^
	effects[name] = efk;
}

void Particle::DeleteEffect(const char* name)
{
}

Effekseer::Handle Particle::Play(const std::string & name, Vec3 pos)
{
	return Effekseer::Handle();
}

void Particle::Stop(Effekseer::Handle handle)
{
}

void Particle::StopRoot(Effekseer::Handle handle)
{
}


void Particle::Update()
{
	// �S�ẴG�t�F�N�g�̍X�V
	manager->Update();
}

void Particle::EffectDraw()
{
	Effekseer::Matrix44 view;
	Effekseer::Matrix44 projection;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			view.Values[y][x] = pCamera->constant.view.r[y].m128_f32[x];
			projection.Values[y][x] = pCamera->constant.projection.r[y].m128_f32[x];
		}
	}
	//=====�d�v===============
	view.Transpose();
	projection.Transpose();
	//=======================

	// ���e�s��̍X�V
	renderer->SetProjectionMatrix(projection);
	// �J�����s��̍X�V
	renderer->SetCameraMatrix(view);

	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}

Particle::~Particle()
{
	manager->StopAllEffects();
	for (auto it : effects) {
		if (it.second == nullptr) {
			continue;
		}
		it.second->Release();
		it.second = nullptr;
	}
	effects.clear();


	// �G�t�F�N�g��������܂��B�Đ����̏ꍇ�́A�Đ����I��������A�����I�ɉ������܂��B
	ES_SAFE_RELEASE(effect);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	manager->Destroy();
	// �T�E���h�p�C���X�^���X��j��
	//sound->Destroy();
	// �`��p�C���X�^���X��j��
	renderer->Destroy();
}
