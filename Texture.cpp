#include "Texture.h"

namespace DX11
{

	Texture::Texture()
	{
		pSampleLiner = nullptr;
		pTexture = nullptr;
	}


	Texture::~Texture()
	{
		if (pSampleLiner != nullptr)
		{
			RELEASE(pSampleLiner);
		}
		if (pTexture != nullptr)
		{
			RELEASE(pTexture);
		}
	}

	bool Texture::Create()
	{
		HRESULT hr;
		hr = D3DX11CreateShaderResourceViewFromFile(
			Device::GetInstace()->pDevice,
			"test.png",
			NULL,
			NULL,
			&pTexture,
			NULL
		);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`���[�̓ǂݍ��݂Ɏ��s", "error", S_OK);
			return false;
		}

		//�T���v���[�쐬
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory(&samDesc, sizeof(D3D11_SAMPLER_DESC));
		samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//���b�s���O���[�h�ɂ���ƁA�摜���J��Ԃ��~���l�߂���悤�Ƀ}�b�s���O�����
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	//�e�N�X�`���[�����b�v���[�h�Ń}�b�s���O
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	//�e�N�X�`���[�����b�v���[�h�Ń}�b�s���O
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	//�e�N�X�`���[�����b�v���[�h�Ń}�b�s���O
		hr = Device::GetInstace()->pDevice->CreateSamplerState(&samDesc, &pSampleLiner);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�T���v���[�̍쐬�Ɏ��s", "error", S_OK);
			return false;
		}


		
		return true;
	}

	void Texture::Draw()
	{
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		//�T���v���[�͂��̃e�N�X�`���[����F���ǂ����o�����̎w��
		Device::GetInstace()->pDeviceContext->PSSetSamplers(
			0,	//�f�o�C�X�̔z��̒��ŃT���v���[�̐ݒ���J�n����ʒu�́A0 ����n�܂�C���f�b�N�X
			1,	//�z����̃T���v���[�̐��B�ő�16
			&pSampleLiner);
		Device::GetInstace()->pDeviceContext->PSSetShaderResources(
			0,	//�f�o�C�X�̔z��̒��ŃV�F�[�_�[ ���\�[�X�̐ݒ���J�n����ʒu�́A0 ����n�܂�C���f�b�N�X
			1,	//�ݒ肷��V�F�[�_�[ ���\�[�X�̐��B�ő�128
			&pTexture);
	}

}
