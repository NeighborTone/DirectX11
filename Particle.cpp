#include "Particle.h"
#include "Engine.h"

Particle::Particle(const char* path):
	renderer(nullptr),
	manager(nullptr),
	effect(nullptr),
	handle(0)
{
	Create();
	Load(path);
	
}

Particle::Particle():
	renderer(nullptr),
	manager(nullptr),
	effect(nullptr),
	handle(0)
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

void Particle::Play(Vec3& pos)
{
	manager->Play(effect, pos.x, pos.y, pos.z);
}

void Particle::Draw(Camera& camera)
{

	Update();
	EffectDraw(camera);
}


void Particle::Update()
{
	// �S�ẴG�t�F�N�g�̍X�V
	manager->Update();
}

void Particle::EffectDraw(Camera& camera)
{
	Effekseer::Matrix44 view;
	Effekseer::Matrix44 projection;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			  view.Values[y][x] = camera.constant.view.r[y].m128_f32[x];
			  projection.Values[y][x] = camera.constant.projection.r[y].m128_f32[x];
		}
	}
	//=====�d�v=====
	view.Transpose();
	projection.Transpose();
	//============

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

	// �G�t�F�N�g��������܂��B�Đ����̏ꍇ�́A�Đ����I��������A�����I�ɉ������܂��B
	ES_SAFE_RELEASE(effect);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	manager->Destroy();
	// �T�E���h�p�C���X�^���X��j��
	//sound->Destroy();
	// �`��p�C���X�^���X��j��
	renderer->Destroy();
}

void Particle::Load(const char* path)
{
	// �G�t�F�N�g�̓Ǎ�
	EFK_CHAR pass[64];
	Effekseer::ConvertUtf8ToUtf16((int16_t*)pass, 64, (const int8_t*)path);
	effect = Effekseer::Effect::Create(manager, pass);
}

