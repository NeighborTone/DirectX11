#include "Engine/Engine.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Particle.h"
#include "Engine/Utilitys/Console.hpp"
#include "Engine/Utilitys/Easing.hpp"
#include "Engine/Sounds/SoundSource.h"
#include "Engine/Utilitys/Counter.hpp"
#include "Engine/Utilitys/Randam.hpp"
//TODO:
//:物理エンジン管理クラスの使い勝手が悪いので何とかする
//:柔軟な衝突検知
//:プリミティブの追加
//:サウンドエフェクトクラスを柔軟にする
//:警告レベル4での警告を最小限にする
//:パッド、マウス、キー入力のグローバル変数の解消
//:モデルに複数のテクスチャをアタッチできるようにする
//:スカイボックス
//:その他のモデル形式の読み込み
//:スプライトアニメーション
//:ライトの配置
//:フルスクリーンで画面がゆがむ
//:フォグ
//:昔に作った機能のリファクタリング
//:書いてないところのDoxy形式のコメント文追加

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ShowConsole();
	//ゲームエンジン生成
	Engine ge("DirectX11", 640, 450, true);
	
	//カメラ生成
	Camera camera3D;
	camera3D.pos.z = -20;
	camera3D.pos.y = 5;
	camera3D.angle.x = 0;

	camera3D.SetPerspective(45.0f, 1, 10000.0f);
	camera3D.SetDepthTest(true);

	Camera camera2D;
	camera2D.SetDepthTest(false);
	camera2D.SetOrthographic(1, 0.1f, 1000.0f);
	
	Texture tex("Resource/F01_512.jpg");
	Texture tex2("Resource/box.jpg");
	Model m("Resource/untitled.fbx");
	m.scale = 0.1f;
	m.angle.x = -90;
	m.angle.y = -90;

	Easing ease;
	Counter cnt(0, 1, 0, 60);
	Particle effect;
	effect.Load("Resource/TestParticle.efk");
	Text text("2DHogeText",20.0f,"メイリオ");
	Random rand;
	Sprite sp("Resource/カーソル.png");
	Mesh mm;
	mm.CreateSphere();
	mm.GetMaterial().SetTexture(0,&tex2);

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
	
		if (KeyBoard::On(KeyBoard::Key::KEY_D)||
			DInput::StickOn(DInput::Stick::RIGHT))
		{
			m.pos.x += 1.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_A) ||
			DInput::StickOn(DInput::Stick::LEFT))
		{
			m.pos.x -= 1.6f;
		}
		if (DInput::StickOn(DInput::Stick::UP_RIGHT))
		{
			m.pos.x += static_cast<float>(cosf((float)M_PI / 180.0f * 45)) * 1.6f;
			m.pos.y += static_cast<float>(sinf((float)M_PI / 180.0f * 45)) * 1.6f;
		}
		if (DInput::StickOn(DInput::Stick::DOWN_RIGHT))
		{
			m.pos.x += static_cast<float>(cosf((float)M_PI / 180.0f * 45)) * 1.6f;
			m.pos.y -= static_cast<float>(sinf((float)M_PI / 180.0f * 45)) * 1.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_W) ||
			DInput::StickOn(DInput::Stick::UP))
		{
			m.pos.y += 1.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_S) ||
			DInput::StickOn(DInput::Stick::DOWN))
		{
			m.pos.y -= 1.6f;
		}
		if (DInput::StickOn(DInput::Stick::UP_LEFT))
		{
			m.pos.x -= static_cast<float>(cosf((float)M_PI / 180.0f * 45)) * 1.6f;
			m.pos.y += static_cast<float>(sinf((float)M_PI / 180.0f * 45)) * 1.6f;
		}
		if (DInput::StickOn(DInput::Stick::DOWN_LEFT))
		{
			m.pos.x -= static_cast<float>(cosf((float)M_PI / 180.0f * 45)) * 1.6f;
			m.pos.y -= static_cast<float>(sinf((float)M_PI / 180.0f * 45)) * 1.6f;
		}
		mm.Draw();
		effect.Draw(camera3D);
		++cnt;
		if (cnt.GetCurrentCount() >= 60)
		{

			effect.Play();
		}
		camera3D.angle.x += Mouse::GetMousePosCenter().y * 0.3f;
		camera3D.angle.y += Mouse::GetMousePosCenter().x * 0.3f;
		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);
		text.color = Float4(1,0,0,1);
		text.pos.y = ease.bounce.Out(ease.Time(10),200, 10 - 200);
		text.Draw();

		text.color = Float4(0, 1, 0, 1);
		text.pos.y = -10;
		text.Draw();

		sp.pos.x = -100;
		sp.Draw();

		
		tex.Attach(0);
		m.Draw();
		
		Mouse::SetMousePos(0, 0); 
		Mouse::DrawCursor(false);
	}

	//終了
	return 0;

}