#include "Engine/Engine.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Particle.h"
#include "Engine/Utilitys/Console.hpp"
#include "Engine/Utilitys/Easing.hpp"
#include "Engine/Sounds/SoundSource.h"
#include "Engine/Utilitys/Counter.hpp"
#include "Engine/Utilitys/Randam.hpp"
#include "Engine/Graphics/SpriteRender.h"
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

	m.GetMaterial().SetTexture(0,&t);
	m.CreateSphere();
	Particle efs;
	efs.AddEffect("test","Resource/TestParticle.efk");
	efs.AddEffect("test2", "Resource/TestParticle2.efk");

	Counter cnt(0, 1, 0, 60);
	Mesh b;
	b.CreateCube();
	b.GetMaterial().SetTexture(0, &t);
	b.scale = 10;
	b.scale.y = 0.5f;
	Sprite2D ssss("Resource/box.jpg");
	Sprite z("Resource/box.jpg");

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
		if (KeyBoard::Down(KeyBoard::Key::KEY_Z))
			efs.Play("test", Vec3(0, 0, 10));
		if (KeyBoard::Down(KeyBoard::Key::KEY_X))
			efs.Play("test2", Vec3(0, 0, 10));
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
			camera3D.pos.z += 0.5f;
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
			camera3D.pos.z -= 0.5f;

		efs.UpDate(Camera(camera3D));

		
		z.scale = 0.01f;
		z.Draw();
		//===================================//
		//==========2DRendering=================//
		//===================================//
		camera2D.Run(false);
		
		ssss.Draw(Math::Box2D(128, 128, 256, 256), Math::Box2D(0, 0, 256, 256));
		
	}

	//終了
	return 0;

}