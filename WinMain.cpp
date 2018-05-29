#include "Engine.h"
#include "Model.h"


//仮----------------------------------------------------
bool HitBall(Vec3 &c1, Vec3& c2, float r,float r2)
{
	if ((c1.x - c2.x) * (c1.x - c2.x) + ((c1.y - c2.y) * (c1.y - c2.y)) + ((c1.z - c2.z) * (c1.z - c2.z)) <= (r + r) * (r2+ r2))
	{
		return true;
	}
	return false;
}
//2次元限定のスプライトの当たり判定
bool HitSprite(Sprite& s1, Sprite& s2)
{
	Vec3 leftTop1 = s1.pos.OffSetCopy(-(s1.GetSize().x/2.0f),s1.GetSize().y/2.0f,s1.pos.z);			//左上
	Vec3 leftTop2 = s2.pos.OffSetCopy(-(s2.GetSize().x / 2.0f), s2.GetSize().y / 2.0f, s2.pos.z);
	Vec3 buttom1 = s1.pos.OffSetCopy(s1.GetSize().x / 2.0f, -(s1.GetSize().y / 2.0f), s1.pos.z);	//右下
	Vec3 buttom2 = s2.pos.OffSetCopy(s2.GetSize().x / 2.0f, -(s2.GetSize().y / 2.0f), s2.pos.z);
	
	if (leftTop1.x < buttom2.x &&
		leftTop2.x < buttom1.x &&
		leftTop1.y > buttom2.y &&
		leftTop2.y > buttom1.y)
		return true;

	return false;
}
bool HitCircle(Sprite& c1, Sprite& c2)
{
	if ((c1.pos.x - c2.pos.x) * (c1.pos.x - c2.pos.x) +
		((c1.pos.y - c2.pos.y) * (c1.pos.y - c2.pos.y)) <=
		(c1.GetSize().x / 2 + c1.GetSize().y / 2) * (c2.GetSize().x / 2 + c2.GetSize().y / 2))
		return true;
	return false;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	ci_ext::Console();
	ShowConsole();
	constexpr int MAX = 5;
	//ゲームエンジン生成
	Engine ge("DirectX11",640,480,true);
	EntityWorld entity;
	entity.SetupWorld();
	//カメラ生成
	Camera camera;
	camera.pos = Vec3(0.0f, 7.0f, -20.0f);
	camera.SetPerspective(45.0f, 1, 10000.0f);
	//camera.SetOrthographic(0,0.1f,100.0f);
	camera.SetDepthTest(true);
	//camera.color = Float4(1, 1, 1, 1);


	Texture texture1("box.jpg");
	Texture texture2("brick.jpg");
	Texture texture3("brick2.jpg");
	Texture texture4("p.png");
	Mesh mesh;
	mesh.CreateCube();
	mesh.GetMaterial().SetTexture(0,&texture2);

	Mesh ground;
	ground.CreateCube();
	ground.GetMaterial().SetTexture(0, &texture4);
	ground.scale = 10;
	ground.scale.y = 1;
	while (ge.Run())
	{
		camera.Run();
		
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
			camera.pos.x += 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_LEFT))
		{
			camera.pos.x -= 3.5f;
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
			camera.pos.y -= 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_RCONTROL))
		{
			camera.pos.y += 3.5f;
		}
		if (KeyBoard::On(KeyBoard::Key::KEY_SPACE))
		{
			camera.angles.x += 0.5f;
		}
		static bool go = false;
		if (KeyBoard::On(KeyBoard::Key::KEY_S))
		{
			go = true;
		}
		if (go)
		{
			entity.UpDate();
		}
		
		mesh.pos = entity.pBox.get()->GetPosition();
		ground.Draw();
		mesh.Draw();
		

	
		std::cout << Engine::GetFps().GetFrameRate() << std::endl;
	}
	
	//終了
	return 0;

}