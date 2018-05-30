#pragma once
#include <string>
#include "Texture.h"

/*! @class Material
*   @brief  �V�F�[�_�[���g�p���邽�߂̃N���X�ł�
*   @detail ���̃N���X�͊�{�I�ɒP�̂ł͎g�p���܂���BMesh��Sprite�Ŏg�p���܂�
*/
class Material
{
public:
	struct ConstantBuffer
	{
		ConstantBuffer() :ptr(nullptr), buffer(nullptr) {};
		void* ptr;
		ATL::CComPtr<ID3D11Buffer> buffer;
	};
private:
	Texture* textures[10];
	ConstantBuffer constbuff[10];
	ATL::CComPtr<ID3D11VertexShader>vertexShader;
	ATL::CComPtr<ID3D11PixelShader>pixelShader;
	ATL::CComPtr<ID3D11InputLayout>inputLayout;

	void 	Initialize();
	static void 	CompileShader(const std::string &source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out);
public:
	Material();
	~Material();
	Material(const char* const filePath);
	Material(const std::string &source);
	/*!
	* @brief �V�F�[�_�[�t�@�C����ǂݍ��݂܂�
	* @param (filePath) �V�F�[�_�[�t�@�C���̃p�X
	* @detail �����ŃV�F�[�_�[�̃R���p�C�����s���܂�
	*/
	void 	Load(const char* const filePath);
	/*!
	* @brief �}�e���A�����쐬���܂�
	* @param (source) �V�F�[�_�[�̃\�[�X
	* @detail �����ŃV�F�[�_�[�̃R���p�C�����s���܂�
	*/
	void 	Create(const std::string &source);
	/*!
	* @brief �V�F�[�_�[�Ŏg���R���X�^���g�o�b�t�@�[��ݒ肵�܂�
	* @param (slot) �ݒ肷��ԍ�
	* @param (cbuffer) �R���X�^���g�o�b�t�@�[�̃|�C���^(�F�Ƃ�)
	* @param (size) �o�b�t�@�̑傫��
	*/
	void 	SetBuffer(int slot, void* cbuffer, size_t size);
	/*!
	* @brief �V�F�[�_�[�Ŏg���e�N�X�`���[��ݒ肵�܂�
	* @param (slot) �ݒ肷��ԍ�
	* @param (texture)  �e�N�X�`���[�̃|�C���^
	*/
	void 	SetTexture(int slot, Texture* texture);
	/*!
	* @brief �V�F�[�_�[���O���t�B�b�N�XAPI�ɐݒ肵�܂�
	*/
	void 	Attach();

};
