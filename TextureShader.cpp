#include "TextureShader.h"

TextureShader::TextureShader(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName) :
	Shader(device,hwnd,shaderPath,vertexFuncName,pixelFuncName)
{
}

TextureShader::~TextureShader()
{
	if (pSampler)
	{
		RELEASE(pSampler);
	}
}

bool TextureShader::Load(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName)
{
	if (!Super::Load(device, hwnd, shaderPath, vertexFuncName, pixelFuncName))
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		//�e�N�X�`���[�̃T���v�����O���Ɏg�p����t�B���^�����O���\�b�h(����͏k���A�g��Ƀ|�C���g�T���v�����O���g�p���A�~�b�v���x���̃T���v�����O�ɐ��`��Ԃ��g�p)
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.MipLODBias = 0.0f;										//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g
	samplerDesc.MaxAnisotropy = 1;										//Filter ��D3D11_FILTER_ANISOTROPIC�܂���D3D11_FILTER_COMPARISON_ANISOTROPIC���w�肳��Ă���ꍇ�Ɏg�p�����N�����v(�͈͌Œ�)�l
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//�����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�
	samplerDesc.BorderColor[0] = 0;										//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[1] = 0;										//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[2] = 0;										//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[3] = 0;										//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.MinLOD = 0;													//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̉����ł��B0�͍ő傩�ł��ڍׂȃ~�b�v�}�b�v���x��
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;							//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̏��

	//�e�N�X�`���[�̃T���v�����O�����J�v�Z��������T���v���[�X�e�[�g�̍쐬
	hr = device->CreateSamplerState(&samplerDesc, &pSampler);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�T���v���[�X�e�[�g�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	return true;
}

void TextureShader::Begin(ID3D11DeviceContext* context, int indexCount)
{
	context->PSSetSamplers(0, 1, &pSampler);
	Super::Begin(context, indexCount);
}

void TextureShader::End(ID3D11DeviceContext* context)
{
	context->PSSetSamplers(0, 0, NULL);
	Super::End(context);
}

