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
		
	public:
		Texture();
		~Texture();
		ID3D11SamplerState * pSampleLiner;		//�T���v���[�A�e�N�X�`���[����s�N�Z�����ǂ̂悤�Ɏ��o�����̎w��
		ID3D11ShaderResourceView* pTexture;		//�e�N�X�`���[���
		bool Create(const char* path);
		void Draw();
		
	};
}


