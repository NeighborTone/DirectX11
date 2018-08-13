#include "SpriteRender.h"

//!@brief	�R���X�g���N�^
//!@param[in]	filePath	�g�p����摜�t�@�C���p�X
Sprite2D::Sprite2D(const std::string& filePath)
{
	spriteBatch = new DirectX::SpriteBatch(&Engine::GetDXContext3D());

	std::wstring wstr = std::wstring(filePath.begin(), filePath.end());
	LPCWSTR sw = wstr.c_str();
	DirectX::CreateWICTextureFromFile(&Engine::GetDXDevice3D(), sw, &resource, &textureResourceView);

	rotate = 0.0f;
	pivot = DirectX::XMFLOAT2(0, 0);
}

//!@brief	�f�X�g���N�^
Sprite2D::~Sprite2D()
{
	Memory::SafeRelease(textureResourceView);
	Memory::SafeRelease(resource);
	Memory::SafeDelete(spriteBatch);
}


//!@brief	�`��
//!@param[in]	draw	�`��ʒu(x,y,width,height)
//!@param[in]	src		�\�[�X�͈̔�(x,y,width,height)
//!@param[in]	color	�F(r,g,b,a)
void	Sprite2D::Draw(const Math::Box2D& draw, const Math::Box2D& src, const bool isCenter, const Math::Color& color, float layer)
{

	ATL::CComPtr<ID3D11BlendState> blendState;
	blendState.Release();
	D3D11_BLEND_DESC blendDesc;
	SecureZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	//�A���t�@�u�����f�B���O�̐ݒ�
	blendDesc.RenderTarget[0].BlendEnable = true;									//�u�����f�B���O��L��(�܂��͖���) 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;						//�ŏ���RGB�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;				//2�Ԗڂ�RGB�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;							//RGB�f�[�^�\�[�X�̑g�������@���`
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;						//�ŏ��̃A���t�@�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;						//2�Ԗڂ̃A���t�@�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;					//�A���t�@�f�[�^�\�[�X�̑g�������@���`
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//�������݃}�X�N
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Engine::GetDXDevice3D().CreateBlendState(&blendDesc, &blendState);
	Engine::GetDXContext3D().OMSetBlendState(blendState, blendFactor, 0xffffffff);

	//�T���v���[
	samplerState.Release();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		//�e�N�X�`���[�̃T���v�����O���Ɏg�p����t�B���^�����O���\�b�h
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//0�`1�͈̔͊O�ɂ���e�N�X�`���[���W���������邽�߂Ɏg�p����郁�\�b�h
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

	DirectX::FXMVECTOR convertColor = DirectX::XMVectorSet(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
	DirectX::XMFLOAT2	pos(draw.x, draw.y);
	DirectX::XMFLOAT2	scale(draw.width / src.width, draw.height / src.height);
	if (isCenter)
	{
		pivot = DirectX::XMFLOAT2((src.x + src.width) / 2.0f, (src.y + src.height) / 2.0f);
	}
	else
	{
		pivot = DirectX::XMFLOAT2(0, 0);
	}
	DirectX::XMFLOAT2 xPos = { pos.x,pos.y };
	DirectX::XMFLOAT2 xPivot = { pivot.x,pivot.y };
	DirectX::XMFLOAT2 xScale = { scale.x , scale.y };
	spriteBatch->Begin(DirectX::SpriteSortMode_BackToFront, blendState, samplerState, nullptr, nullptr, nullptr);
	spriteBatch->SetViewport(Engine::GetViewPort());
	spriteBatch->Draw(textureResourceView, xPos, &src, convertColor, rotate, xPivot, xScale, DirectX::SpriteEffects_None, layer);
	spriteBatch->End();
}

//!@brief	��]�ʂ̒���
//!@param[in]	rotate	��]��(�x���@)
void	Sprite2D::SetRotation(const float rotate)
{
	this->rotate = rotate;
}