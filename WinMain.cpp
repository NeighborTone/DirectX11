#include "Engine.h"
#include "Model.h"
//仮
bool HitBall(Vec3 &c1, Vec3& c2, float r,float r2)
{
	if ((c1.x - c2.x) * (c1.x - c2.x) + ((c1.y - c2.y) * (c1.y - c2.y)) + ((c1.z - c2.z) * (c1.z - c2.z)) <= (r + r) * (r2+ r2))
	{
		return true;
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ci_ext::Console();
	ShowConsole();
	constexpr int MAX = 5;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);
	
	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 0.0f, -50.0f);
	camera.SetPerspective(45.0f, 1, 10000.0f);
	//camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);
	//camera.color = Float4(1, 1, 1, 1);
	
	//サウンドソースの登録
	SoundSource source;
	source.Load("Grass.wav");
	Engine::GetSoundSystem().AddSource(source);
	source.PlayBGM();

	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");
	Texture texture4("p.png");

	/*Text text("あ",16);
	text.pos.y = 5;
	text.pos.z = 0;
	text.scale = 0.1f;*/
	//std::vector<std::vector<Mesh>> m;
	//m.resize(MAX);						//1次元目の要素数分確保
	//for (int i = 0; i < MAX; ++i)
	//{
	//	m[i].resize(MAX);				//2次元目の要素数分確保
	//}
	//m[0][0].GetMaterial().SetTexture(0, &texture2);
	//for (int y = 0; y < MAX; y++)
	//{
	//	for (int x = 0; x < MAX; x++)
	//	{
	//		m[y][x].CreateCube();
	//		m[y][x].SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	//		m[y][x].pos.x += x + 10;
	//		m[y][x].pos.z += y;
	//	}
	//}
	
	Model model(L"sphere.fbx");
	model.meshes[0]->GetMaterial().SetTexture(0, &texture4);
	for (int i = 0; i < model.meshes.size(); i++) {
		model.meshes[i]->GetMaterial().Load("Shader/test.hlsl");
		model.meshes[i]->SetDrawMode(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	}
	
	//Mesh p[10];
	//p[0].GetMaterial().SetTexture(0, &texture4);
	//for (int i = 0; i < 10; ++i)
	//{
	//	p[i].CreatePoint(Vec3(0, 0, 0));
	//	p[i].SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	//	p[i].pos.x = (float)i;
	//}
	
	while (ge.Run())
	{
		
		if (KeyBoard::Down(KeyBoard::Key::KEY_ESCAPE) ||
			Pad::Down(Pad::Button::PAD_START))
		{
			break;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_UP))
		{
			camera.pos.z += 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_DOWN))
		{
			camera.pos.z -= 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RIGHT))
		{
			camera.pos.x += 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera.pos.x -= 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RSHIFT))
		{
			camera.angles.y += 1.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LSHIFT))
		{
			camera.angles.y -= 1.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LCONTROL))
		{
			camera.pos.y -= 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RCONTROL))
		{
			camera.pos.y += 0.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_SPACE))
		{
			camera.angles.x += 0.5f;
		}
		camera.Run();
		for (int i = 0; i < model.meshes.size(); i++) {
			model.meshes[i]->angle.y += Engine::GetFps().GetDeltaTime() * 50.0f;
		}
		model.Draw();
		//std::cout << Mouse::GetMousePos().x << ": " << Mouse::GetMousePos().y << "\n";
	//	text.Draw();
	/*	for(int y = 0; y < MAX; y++)
		{
			for (int x = 0; x < MAX; x++)
			{
				m[y][x].Draw();
			}
		}*/
	//	text.angle.y += 2;

	/*	for (auto &i : p)
		{
			i.DrawPoint();
		}*/

		
		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}
	
	//終了
	return 0;

}