#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//Memo//
	//Primitiveの設定を描画命令に移したのでスプライトやテキストもそれぞれの描画関数内で設定する
	//


	//ゲームエンジン生成
	Engine ge("Directx11",640,480,true);
	
	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 0.0f, -5.0f);
	//camera.SetPerspective(100.0f, 1, 100.0f);
	camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);
	//camera.color = Float4(1, 1, 1, 1);
	
	//サウンドソースの登録
	SoundSource source;
	source.Load("Grass.wav");
	Engine::GetSoundSystem().AddSource(source);
	//source.PlayBGM();

	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");
	Texture texture4("p.png");

	Mesh mesh;
	mesh.CreateCube();
	mesh.SetDrawMode(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
	mesh.GetMaterial().SetTexture(0, &texture4);
	mesh.scale *= 100;

	Mesh mesh2;
	mesh2.CreateCube();
	mesh2.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	mesh2.GetMaterial().SetTexture(0, &texture2);
	mesh2.scale *= 100;

	Mesh mesh3;
	mesh3.CreateCube();
	mesh3.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	mesh3.GetMaterial().SetTexture(0, &texture3);
	mesh3.scale *= 100;


	float x = 0;
	while (ge.Run())
	{
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			Pad::Down(Pad::Button::PAD_START))
		{
			break;
		}
		
		camera.Update();
		camera.pos.x += 1;

		mesh.angle.y += 1.0f;
		mesh.pos.x = x;
		mesh.Draw();
		x += 1;
	
		mesh2.angle.x += 1.0f;
		mesh2.pos.x = 200.0f;
		mesh2.Draw();

		mesh3.angle.x -= 1.0f;
		mesh3.pos.x = -200.0f;
		mesh3.Draw();

	}
	
	//終了
	return 0;

}