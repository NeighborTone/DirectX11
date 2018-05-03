#include "DirectX.h"

namespace DX11
{

	bool DirectX::Init(System& win)
	{
		device = Direct3D::GetInstace();
		if (!device->Create(win))
		{
			MessageBox(NULL, "�V�X�e���̏������Ɏ��s���܂���", "error", S_OK);
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
		//�`��J�n
		device->BeginDraw();
		tex.Draw();
		device->DrawPrimitive();

		device->Flip();
	}

	void DirectX::End()
	{

	}

}


