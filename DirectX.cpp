#include "DirectX.h"

namespace DX11
{

	bool DirectX::Init(System& win)
	{
		device = Direct3D::GetInstace();
		if (!device->Create(win))
		{
			MessageBox(NULL, "システムの初期化に失敗しました", "error", S_OK);
			return false;
		}
		if (!tex.Create("test.png"))
		{
			return false;
		}
		return true;

	}

	void DirectX::Update()
	{

	}

	void DirectX::Draw()
	{
		//描画開始
		device->BeginDraw();
		tex.Draw();
		device->DrawPrimitive();

		device->Flip();
	}

	void DirectX::End()
	{

	}

}


