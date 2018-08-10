#include "Particle.h"
#include "../Engine.h"
#include <iostream>



Particle::Particle()
{
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


void Particle::SetMatrix(Camera& camera)
{
	pCamera = &camera;
}

void Particle::AddEffect(const std::string name, const char * filePass)
{
	try
	{
		//名前の重複防止
		if (effects.find(name) != effects.end())
		{
			throw name + "は既に登録されています";
		}
	}
	catch(const std::string str)
	{
		std::cerr << str;
		return;
	}

	EFK_CHAR pass[64];
	Effekseer::ConvertUtf8ToUtf16((int16_t*)pass, 64, (const int8_t*)filePass);
	Effekseer::Effect* efk = Effekseer::Effect::Create(manager, pass);
	if (efk == nullptr) 
	{
		return;
	}
	//リストへ登録
	effects[name] = efk;
}

void Particle::DeleteEffect(const char* name)
{
	if (effects.find(name) == effects.end() || !effects[name]) 
	{
		return;
	}
	effects[name]->Release();
	effects.erase(name);
}

Effekseer::Handle Particle::Play(const std::string & name, Vec3&& pos)
{
	//リストにその名前があってインスタンスもあるものが対象
	if (effects.find(name) == effects.end() || !effects[name]) 
	{
		return -1;
	}

	return manager->Play(effects[name], pos.x, pos.y, pos.z);
}

void Particle::Stop(Effekseer::Handle handle)
{
	manager->StopEffect(handle);
}

void Particle::StopRoot(Effekseer::Handle handle)
{
	manager->StopRoot(handle);
}

void Particle::SetPos(Effekseer::Handle handle, Vec3 && pos)
{
	manager->SetLocation(handle, pos.x, pos.y, pos.z);
}

void Particle::SetScale(Effekseer::Handle handle, Vec3 && scale)
{
	manager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Particle::SetAngles(Effekseer::Handle handle, Vec3 && angles)
{
	manager->SetRotation(handle,
		DirectX::XMConvertToRadians(angles.x),
		DirectX::XMConvertToRadians(angles.y), 
		DirectX::XMConvertToRadians(angles.z));
}


void Particle::UpDate(Camera&& camera)
{
	// 全てのエフェクトの更新
	manager->Update();
	SetMatrix(camera);
	EffectDraw();
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

Particle::~Particle()
{
	manager->StopAllEffects();
	for (auto& it : effects)
	{
		if (it.second == nullptr) 
		{
			continue;
		}
		it.second->Release();
		it.second = nullptr;
	}
	effects.clear();

	// エフェクト管理用インスタンスを破棄
	manager->Destroy();
	// サウンド用インスタンスを破棄
	//sound->Destroy();
	// 描画用インスタンスを破棄
	renderer->Destroy();
}
