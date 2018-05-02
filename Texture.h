#pragma once
#pragma warning( disable : 4005 )	//DirectX�}�N���Ē�`
#include <d3dx11.h>
#include <d3d11.h>
#include "Device.h"
namespace DX11
{
	class Texture
	{
	private:
		Texture();
	public:
		ID3D11SamplerState * pSampleLiner;		//�T���v���[�A�e�N�X�`���[����s�N�Z�����ǂ̂悤�Ɏ��o�����̎w��
		ID3D11ShaderResourceView* pTexture;		//�e�N�X�`���[���
		bool Create();
		void Draw();
		static Texture* GetInst()
		{
			static Texture inst;
			return &inst;
		}
		~Texture();
	};
}


