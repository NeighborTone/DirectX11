#pragma once
#include "Device.h"
#include "System.h"
#include "Texture.h"
namespace DX11
{
	class DirectX final
	{
	private:
		DX11::Direct3D* device;
		DX11::Texture tex;
	public:
		bool Init(System& win);
		void Update();
		void Draw();
		void End();
		DirectX() = default;
		~DirectX() = default;
	};
}


