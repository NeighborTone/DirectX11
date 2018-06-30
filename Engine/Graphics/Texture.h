#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <atlbase.h>
#include <string>

/*! @class Texture
*   @brief  �摜���g�p���邽�߂̃N���X�ł�
*   @detail ���̃N���X�͊�{�I�ɒP�̂ł͎g�p���܂���BMesh��Sprite�Ŏg�p���܂�
*/
class Texture
{
private:
	DirectX::XMINT2 size;
	ATL::CComPtr<ID3D11Texture2D> texture;
	ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView;
	ATL::CComPtr<ID3D11SamplerState> samplerState;
public:
	Texture();
	~Texture();
	Texture(const std::string filePath);
	Texture(const BYTE* const buffer, int width, int height);

	/*!
	* @brief �摜��ǂݍ��݂܂��B
	* @param (filepath) �t�@�C���p�X
	*/
	void Load(const std::string filePath);
	/*!
	* @brief �e�N�X�`�����쐬���܂�
	* @param (buffer)�@�o�b�t�@
	* @param (width)    ��
	* @param (height)   ����
	*/
	void Create(const BYTE* const buffer, int width, int height);
	/*!
	* @brief  �e�N�X�`���̃T�C�Y���擾���܂��B
	* @return XMINT2 
	*/
	DirectX::XMINT2 GetSize() const;
	/*!
	* @brief �e�N�X�`�����O���t�B�b�N�XAPI�ɐݒ肵�܂�
	* @param (sloat) GPU�ɐݒ肷��ꏊ�ł��B�V�F�[�_�[�ɓn���܂�
	*/
	void Attach(int slot);
	ID3D11Texture2D& GetInterface();
};