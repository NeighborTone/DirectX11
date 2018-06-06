#pragma once
#include "Utility.hpp"
#include "Material.h"
#include <DirectXMath.h>
#include <vector>

/*! @class Mesh
*   @brief �|���S���̏W�܂���g�p���邽�߂̃N���X�ł�
*   @detail cube�ƕ��ʂ��`��ł��܂��ASetTexture�Ń��\�[�X���Z�b�g���Ă�������
*/
class Mesh
{
private:
	struct Constant
	{
		DirectX::XMMATRIX world;
	};

	Material material;
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer;
	ATL::CComPtr<ID3D11Buffer> indexBuffer;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState;
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	Mesh();
	~Mesh();
	/*!
	* @brief �_���쐬���܂�
	* @param (p1)	�|���S���̏����ʒu
	* @param (offset) �|���S���̃I�t�Z�b�g�l
	* @param (shouldClear) ���_�A�C���f�b�N�X�̏�����
	* @param (forwardDirection) �@���̌���
	*/
	void CreatePoint(
		Vec3 p1,
		Vec3 offset = Vec3(0.0f, 0.0f, 0.0f),
		Vec3 forwardDirection = Vec3(0.0f, 0.0f, 1.0f),
		bool shouldClear = true
		);

	/*!
	* @brief ���ʂ��쐬���܂�
	* @param (size)	�|���S���̃T�C�Y
	* @param (offset) �|���S���̏����ʒu
	* @param (shouldClear) ���_�A�C���f�b�N�X�̏�����
	* @param (leftDirection) �������̓_
	* @param (upDirection) ������̓_
	* @param (forwardDirection) �@���̌���
	*/
	void CreatePlane(
		Vec2 size,
		Vec3 offset = Vec3(0.0f, 0.0f, 0.0f), 
		bool shouldClear = true, 
		Vec3 leftDirection = Vec3(1.0f, 0.0f, 0.0f), 
		Vec3 upDirection = Vec3(0.0f, 1.0f, 0.0f), 
		Vec3 forwardDirection = Vec3(0.0f, 0.0f, 1.0f));
	/*!
	* @brief �l�ʑ̂��쐬���܂�
	* @param (shouldClear) ���_�A�C���f�b�N�X�̏�����
	*/
	void CreateCube(bool shouldClear = true);
	/*!
	* @brief Material���擾���܂�
	* @return Material&
	*/
	Material& GetMaterial();
	/*!
	* @brief �`��^�C�v���w�肵�܂�
	* @param (cullingMode) �J�����O�̐ݒ�,D3D11_CULL_MODE
	* @param (fillMode) ���b�V���̕`��I�v�V����,D3D11_FILL_MODE
	*/
	void SetDrawMode(D3D11_CULL_MODE cullingMode, D3D11_FILL_MODE fillMode);
	/*!
	* @brief �ύX�������_���p�C�v���C���ɗ����܂�
	*/
	void Apply();
	/*!
	* @brief Mesh��`�悵�܂�
	*/
	void Draw();
	/*!
	* @brief �_��`�悵�܂�
	*/
	void DrawPoint();

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

