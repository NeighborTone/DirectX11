#include "Texture.h"
#include "Engine.h"
#include "Utility.hpp"
#include <memory>
Texture::Texture():
	texture(nullptr),
	shaderResourceView(nullptr),
	samplerState(nullptr)
{
	Engine::COMInitialize();
}

Texture::Texture(const char * const filePath)
{
	Engine::COMInitialize();
	Load(filePath);
}

Texture::Texture(const BYTE * const buffer, int width, int height)
{
	Engine::COMInitialize();
	Create(buffer, width, height);
}


Texture::~Texture()
{

}

void Texture::Load(const char* const filePath)
{
	HRESULT hr;
	ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;
	hr = Engine::GetTextureFactory().CreateDecoderFromFilename(
		(LPCWSTR)filePath,
		0,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	ErrorMessage(hr,"�e�N�X�`���[�̓ǂݍ��݂Ɏ��s","Error");

	ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
	decoder->GetFrame(0, &frame);
	UINT width, height;
	frame->GetSize(&width, &height);

	WICPixelFormatGUID pixelFormat;
	hr = frame->GetPixelFormat(&pixelFormat);
	ErrorMessage(hr, "�e�N�X�`���[�̌`���擾�Ɏ��s", "Error");

	std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);

	if (pixelFormat != GUID_WICPixelFormat32bppBGRA)
	{
		//Direct2D�̓r�b�g�}�b�v�����`��ł��Ȃ��̂�png����ϊ����s��
		ATL::CComPtr<IWICFormatConverter> formatConverter = nullptr;
		hr = Engine::GetTextureFactory().CreateFormatConverter(&formatConverter);
		ErrorMessage(hr,"�R���o�[�^�[�̍쐬�Ɏ��s","Error");
	    
		hr =	formatConverter->Initialize(
			frame, 
			GUID_WICPixelFormat32bppBGRA, 
			WICBitmapDitherTypeErrorDiffusion, 
			0, 
			0, 
			WICBitmapPaletteTypeCustom);
		ErrorMessage(hr, "�e�N�X�`���[�̃R���o�[�g�Ɏ��s", "Error");

		hr = formatConverter->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		ErrorMessage(hr, "�e�N�X�`���[���̎擾�Ɏ��s", "Error");
	}
	else
	{
		hr = frame->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		ErrorMessage(hr, "�e�N�X�`���[���̎擾�Ɏ��s", "Error");
	}

	Create(buffer.get(), width, height);
}

void Texture::Create(const BYTE * const buffer, int width, int height)
{
	size = DirectX::XMINT2(width, height);

	// �����_�����O�e�N�X�`�����쐬����
	texture.Release();
	D3D11_TEXTURE2D_DESC textureDesc;
	SecureZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
	textureSubresourceData.pSysMem = buffer;
	textureSubresourceData.SysMemPitch = width * 4;
	textureSubresourceData.SysMemSlicePitch = width * height * 4;
	Engine::GetDXDevice3D().CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);

	//ShaderResourceView���쐬����
	//������쐬���Ă����Ȃ��ƃe�N�X�`�����\�[�X�Ƃ��Ďg�p�ł��Ȃ�
	shaderResourceView.Release();
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	Engine::GetDXDevice3D().CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView);

	//�T���v���[�̍쐬
	samplerState.Release();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		//�e�N�X�`���[�̃T���v�����O���Ɏg�p����t�B���^�����O���\�b�h
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.MipLODBias = 0.0f;													//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g
	samplerDesc.MaxAnisotropy = 1;													//Filter ��D3D11_FILTER_ANISOTROPIC�܂���D3D11_FILTER_COMPARISON_ANISOTROPIC���w�肳��Ă���ꍇ�Ɏg�p�����N�����v(�͈͌Œ�)�l
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//�����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�
	samplerDesc.BorderColor[0] = 0.0f;											//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[1] = 0.0f;											//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[2] = 0.0f;											//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.BorderColor[3] = 0.0f;											//AddressU�AAddressV�AAddressW��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F�B0.0�`1.0�͈̔͂Ŏw��
	samplerDesc.MinLOD = 0.0f;														//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̉����ł��B0�͍ő傩�ł��ڍׂȃ~�b�v�}�b�v���x��
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                          //�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̏��
	Engine::GetDXDevice3D().CreateSamplerState(&samplerDesc, &samplerState);
}

DirectX::XMINT2 Texture::GetSize() const
{
	return size;
}

void Texture::Attach(int slot)
{
	if (texture == nullptr)
	{
		return;
	}
	Engine::GetDXContext3D().PSSetShaderResources(slot, 1, &shaderResourceView.p);
	Engine::GetDXContext3D().PSSetSamplers(slot, 1, &samplerState.p);
}

ID3D11Texture2D & Texture::GetInterface()
{
	return *texture;
}
