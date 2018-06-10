#include "Effecter.h"
#include "Engine.h"


Effecter::Effecter()
{

	// �`��Ǘ��C���X�^���X�̐���
	renderer = EffekseerRendererDX11::Renderer::Create(
		&Engine::GetDXDevice3D(),
		&Engine::GetDXContext3D(),
		1000);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	manager = Effekseer::Manager::Create(1000);
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
	// �G�t�F�N�g�̓Ǎ�
	EFK_CHAR pass[64];
	Effekseer::ConvertUtf8ToUtf16((int16_t*)pass, 64, (const int8_t*)"Resource/testEf.efk");
	effect = Effekseer::Effect::Create(manager, pass);


	handle = manager->Play(effect, 0, 3, 0);
	
}

void Effecter::Play()
{
	manager->Play(effect, 0, 0, 0);
}

void Effecter::Run()
{
	
	// 3D�T�E���h�p���X�i�[�ݒ�̍X�V
	//sound->SetListener(���X�i�[�ʒu, ���ړ_, ������x�N�g��);
	// �S�ẴG�t�F�N�g�̍X�V
	manager->Update();
}

void Effecter::Draw(Camera& camera,DirectX::XMMATRIX v, DirectX::XMMATRIX p)
{
	Effekseer::Matrix44 view;
	Effekseer::Matrix44 projection;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			  view.Values[y][x] = v.r[y].m128_f32[x];
			  projection.Values[y][x] = p.r[y].m128_f32[x];
		}
	}
	view.LookAtLH(
		Effekseer::Vector3D(
			camera.pos.x, 
			camera.pos.y,
			camera.pos.z),
		Effekseer::Vector3D(
			0,
			0,
			0),
		Effekseer::Vector3D(0,1,0));
	projection.PerspectiveFovLH(
		DirectX::XMConvertToRadians(45), Engine::GetWindowSize().x / (FLOAT)Engine::GetWindowSize().y, 1, 10000.0f
	);

	// ���e�s��̍X�V
	renderer->SetProjectionMatrix(projection);
	// �J�����s��̍X�V
	renderer->SetCameraMatrix(view);
	
	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}


Effecter::~Effecter()
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
