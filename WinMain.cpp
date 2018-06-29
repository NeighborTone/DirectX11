#include "Engine.h"
#include "Model.h"
#include "Particle.h"
#include "Console.hpp"
#include "Easing.hpp"
#include "XInput.h"
#include "SoundSource.h"
#include "Counter.hpp"
#include "Randam.hpp"
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
	Engine ge("DirectX11", 1280, 900, true);
	
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
	
	Model m("Resource/untitled.fbx");
	m.scale = 0.01f;
	m.angle.x = -90;
	m.angle.y = -90;

	Easing ease;
	Counter cnt(0, 1, 0, 60);
	Particle effect;
	effect.Load("Resource/TestParticle2.efk");
	Text text("2DHogeText",20.0f,"メイリオ");
	Random rand;
	while (ge.Run())
	{
		
		std::cout <<rand.GetRand(0,1)<< std::endl;
		//===================================//
		//==========3DRendering=================//
		//===================================//
		camera3D.Run(true);
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE))
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
		if (KeyBoard::On(KeyBoard::Key::KEY_D))
		{
			m.pos.x += 0.6f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_A))
		{
			m.pos.x -= 0.6f;
		}
		tex.Attach(0);
		m.Draw();

		effect.Draw(camera3D);
		++cnt;
		if (cnt.GetCurrentCount() >= 60)
		{

			effect.Play();
		}
		
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
		//std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}

	//終了
	return 0;

}