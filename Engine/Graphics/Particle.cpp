#include "Particle.h"
#include "../Engine.h"

Particle::Particle(const char* path)
{
	Init();
	Create();
	Load(path);

}

Particle::Particle()
{
	Init();
	Create();
}

void Particle::Create()
{
	// 描画管理インスタンスの生成
	renderer = EffekseerRendererDX11::Renderer::Create(
		&Engine::GetDXDevice3D(),
		&Engine::GetDXContext3D(),
		MAX_SPRITS);
	// エフェクト管理用インスタンスの生成
	manager = Effekseer::Manager::Create(MAX_SPRITS);
	// 座標系の指定(RHで右手系、LHで左手系)
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	// 描画方法を指定します。独自に拡張することもできます。
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
	manager->SetTextureLoader(renderer->CreateTextureLoader());
}

void Particle::Play()
{
	handle = manager->Play(effect, pos.x, pos.y, pos.z);
	manager->SetScale(handle, scale.x, scale.y, scale.z);
	manager->SetRotation(
		handle, 
		DirectX::XMConvertToRadians(angle.x), 
		DirectX::XMConvertToRadians(angle.y), 
		DirectX::XMConvertToRadians(angle.z)
	);
}

void Particle::Stop()
{
	manager->StopEffect(handle);
}

void Particle::Draw(Camera& camera)
{
	Update();
	EffectDraw(camera);
}


void Particle::Update()
{
	// 全てのエフェクトの更新
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
	//=====重要===============
	view.Transpose();
	projection.Transpose();
	//=======================

	// 投影行列の更新
	renderer->SetProjectionMatrix(projection);
	// カメラ行列の更新
	renderer->SetCameraMatrix(view);

	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}

void Particle::Init()
{
	pos = 0;
	scale = 1;
	angle = 0;
	renderer = nullptr;
	manager = nullptr;
	effect = nullptr;
	handle = 0;
}

Particle::~Particle()
{

	// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
	ES_SAFE_RELEASE(effect);
	// エフェクト管理用インスタンスを破棄
	manager->Destroy();
	// サウンド用インスタンスを破棄
	//sound->Destroy();
	// 描画用インスタンスを破棄
	renderer->Destroy();
}

void Particle::Load(const char* path)
{
	// エフェクトの読込
	EFK_CHAR pass[64];
	Effekseer::ConvertUtf8ToUtf16((int16_t*)pass, 64, (const int8_t*)path);
	effect = Effekseer::Effect::Create(manager, pass);
}

