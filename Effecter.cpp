#include "Effecter.h"
#include "Engine.h"


Effecter::Effecter()
{

	// 描画管理インスタンスの生成
	renderer = EffekseerRendererDX11::Renderer::Create(
		&Engine::GetDXDevice3D(),
		&Engine::GetDXContext3D(),
		1000);
	// エフェクト管理用インスタンスの生成
	manager = Effekseer::Manager::Create(1000);
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
	// エフェクトの読込
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
	
	// 3Dサウンド用リスナー設定の更新
	//sound->SetListener(リスナー位置, 注目点, 上方向ベクトル);
	// 全てのエフェクトの更新
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

	// 投影行列の更新
	renderer->SetProjectionMatrix(projection);
	// カメラ行列の更新
	renderer->SetCameraMatrix(view);
	
	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}


Effecter::~Effecter()
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
