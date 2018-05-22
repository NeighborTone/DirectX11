#pragma once
#include "Utility.hpp"
#include "Mesh.h"
#include <DirectXMath.h>

/*! @class Sprite
*   @brief �摜��`�悷�邽�߂̃N���X�ł�
*/
class Sprite
{
protected:
	Mesh mesh;
	Texture texture;
	void Initialize();
	void Create(const BYTE* const buffer, int width, int height);
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;
	Float4 color;

	Sprite();
	Sprite(const char* const filePath);
	Sprite(const BYTE* const buffer, int width, int height);
	~Sprite();

	/*!
	* @brief �X�v���C�g�����[�h���܂�
	* @param (filePath) �摜�̃p�X
	*/
	void Load(const char* const filePath);

	/*!
	* @brief �X�v���C�g�̃T�C�Y�𓾂܂�
	* @return XINT2
	*/
	DirectX::XMINT2 GetSize() const;
	/*!
	* @brief �X�v���C�g�̎���ݒ肵�܂�
	* @param (pivot) ��]�̒��S��
	*/
	void SetPivot(Vec2 pivot);
	/*!
	* @brief Material���擾���܂�
	* @return Material&
	*/
	Material& GetMaterial();
	/*!
	* @brief �X�v���C�g��`�悵�܂�
	*/
	void Draw();
};

