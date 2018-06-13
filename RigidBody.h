#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"
//�Ƃ肠�����e���v���[�g���\�b�h�p�^�[���Ŏ���


/*!
* @enum Axis
* SetAxisAndAngle���\�b�h�̑������Ŏg���܂�
* ����͂ǂ̎��ɑ΂��ĉ�]�����������w�肷�邽�߂ɗp���܂�
*/
enum class Axis
{
	X,
	Y,
	Z
};

/*! 
*	@class RigidBody
*   @brief  OpenDynamicsEngine�ŕ������Z���s���������̂𐶐�����N���X�̃C���^�[�t�F�[�X�ł�
*   @detail ���̃N���X��PhysicsWorld��AddRigidBody���\�b�h�̈����Ő������܂�
*	���̃N���X�P�̂ł͎g�p�ł��܂���
*	���̃N���X�������\�b�h�̏������e�͕`��ȊO�p����ł��ς��܂���
*/
class RigidBody
{
private:

protected:
	dBodyID body;		//����
	dGeomID geom;		//�Փˌ��m
	
public:
	RigidBody();
	virtual ~RigidBody();
	/*!
	* @brief dGeomID���擾���܂�
	* @return dGeomID
	*/
	dGeomID GetGeomID() const;
	/*!
	* @brief dBodyID���擾���܂�
	* @return dBodyID
	*/
	dBodyID GetBodyID() const;
	/*!
	* @brief ���W���擾���܂�
	* @return Vec3
	*/
	Vec3 GetPosition() const;
	/*!
	* @brief ���W��ݒ肵�܂�
	* @param (pos) ���W
	*/
	void SetPosition(const Vec3& pos);
	/*!
	* @brief �{�f�B�̏d�S�ɍ�p����͂�ݒ肵�܂��B����͎�Ƀ{�f�B��L��������Ƃ��Ɏg�p���܂��B
	* @detail ���̃��\�b�h��p���邱�ƂŁA���̂��Ƃɏd�͂�������悤�Ȃ��Ƃ��ł��܂�
	* @param (force) �����ɑ΂��ė^��������
	*/
	void AddForce(const Vec3& force);
	/*!
	* @brief ���̂ɑ��x��^���܂�
	* @param (velocity) �����ɑ΂��ė^���������x
	*/
	void AddVelocity(const Vec3& velocity);
	/*!
	* @brief ���̂̑��x�𓾂܂�
	* @return Vec3
	*/
	Vec3 GetVelocity() const;
	/*!
	* @brief ������
	*/
	void SetQuaternion(Vec3& angle);
	/*!
	* @brief ������
	*/
	Vec3 GetQuaternion();
	/*!
	* @brief �C�ӂ̎�1�ɑ΂��Ă̎p����ݒ肵�܂�
	* @param (axis) ��
	* @param (degree) ��]�l
	*/
	void SetAxisAndAngle(Axis axis,float degree);
	/*!
	* @brief ���̂ɑ΂��Ă̎p����ݒ肵�܂�
	* @param (angle) �x��(degree)�ł̉�]�l
	*/
	void SetRotation(const Vec3& angle);
	/*!
	* @brief ���̂̎p������]�s��œ��܂�
	* @return DirectX::XMMATRIX
	*/
	DirectX::XMMATRIX GetRotation() const;
	/*!
	* @brief ���̂̕������Z��L���ɂ��܂�
	*/
	void BodyEnable();
	/*!
	* @brief ���̂̕������Z�𖳌��ɂ��܂�
	*/
	void BodyDisable();
	/*!
	* @brief ���̂̌`���`�悵�܂�(����Box�̂�)
	* @param (tex) �`��Ɏg�������e�N�X�`���[
	*/
	virtual void Draw(Texture& tex) = 0;
};


/*! 
*	@class DynamicBox
*   @brief �����̂𐶐����܂�
*/
class DynamicBox : public RigidBody
{
private:
	Mesh mesh;
public:
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
	/*!
	* @brief �����̂𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (scale)	 ���̂̂��ꂼ��̎���1�ӂ̑傫��
	* @param (totalMass) ����
	*/
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);

	void Draw(Texture& tex) override;

	//�A���C�����g�΍�
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

/*! 
*	@class DynamicSphere
*   @brief ���𐶐����܂�
*/
class DynamicSphere : public RigidBody
{
public:
	DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	/*!
	* @brief ���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (radius)    ���a
	* @param (totalMass) ����
	*/
	void Create(const Vec3& pos, const dReal& radius, const dReal totalMass);
	void Draw(Texture& tex) override;
};

/*! 
*	@class DynamicCylinder
*   @brief �~���𐶐����܂�
*/
class DynamicCylinder : public RigidBody
{
public:
	DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCylinder(const DynamicCylinder& cylinder);
	DynamicCylinder();
	~DynamicCylinder();
	/*!
	* @brief �~���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (totalMass) ����
	* @param (direction) �~���̌���
	* @param (radius)    ���a
	* @param (length)    ����
	*/
	void Create(const Vec3& pos , const dReal totalmass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
 
/*!
*	@class DynamicCapsule
*   @brief �J�v�Z���𐶐����܂�
*/
class DynamicCapsule : public RigidBody
{
public:
	DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCapsule(const DynamicCapsule& capsule);
	DynamicCapsule();
	~DynamicCapsule();
	/*!
	* @brief �J�v�Z���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (totalMass) ����
	* @param (direction) �J�v�Z���̌���
	* @param (radius)    ���a
	* @param (length)    ����
	*/
	void Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
