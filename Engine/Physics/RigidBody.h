#pragma once
#include "Physics.h"
#include "../Utilitys/Vec.hpp"
#include "../Graphics/Mesh.h"
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
	virtual dGeomID GetGeomID() const final;
	/*!
	* @brief dBodyID���擾���܂�
	* @return dBodyID
	*/
	virtual dBodyID GetBodyID() const final;
	/*!
	* @brief ���W���擾���܂�
	* @return Vec3
	*/
	virtual Vec3 GetPosition() const final;
	/*!
	* @brief ���W��ݒ肵�܂�
	* @param (pos) ���W
	*/
	virtual void SetPosition(const Vec3& pos) final;
	/*!
	* @brief �{�f�B�̏d�S�ɍ�p����͂�ݒ肵�܂��B����͎�Ƀ{�f�B��L��������Ƃ��Ɏg�p���܂��B
	* @detail ���̃��\�b�h��p���邱�ƂŁA���̂��Ƃɏd�͂�������悤�Ȃ��Ƃ��ł��܂�
	* @param (force) �����ɑ΂��ė^��������
	*/
	virtual void AddForce(const Vec3& force) final;
	/*!
	* @brief ���̂ɑ��x��^���܂�
	* @param (velocity) �����ɑ΂��ė^���������x
	*/
	virtual void AddVelocity(const Vec3& velocity) final;
	/*!
	* @brief ���̂̑��x�𓾂܂�
	* @return Vec3
	*/
	virtual Vec3 GetVelocity() const final;
	/*!
	* @brief ������
	*/
	virtual void SetQuaternion(Vec3& angle) final;
	/*!
	* @brief ������
	*/
	virtual Vec3 GetQuaternion() final;
	/*!
	* @brief �C�ӂ̎�1�ɑ΂��Ă̎p����ݒ肵�܂�
	* @param (axis) ��
	* @param (degree) ��]�l
	*/
	virtual void SetAxisAndAngle(Axis axis,float degree) final;
	/*!
	* @brief ���̂ɑ΂��Ă̎p����ݒ肵�܂�
	* @param (angle) �x��(degree)�ł̉�]�l
	*/
	virtual void SetRotation(const Vec3& angle) final;
	/*!
	* @brief ���̂̎p������]�s��œ��܂�
	* @return DirectX::XMMATRIX
	*/
	virtual DirectX::XMMATRIX GetRotation() const final;
	/*!
	* @brief ���̂̕������Z��L���ɂ��܂�
	*/
	virtual void BodyEnable() final;
	/*!
	* @brief ���̂̕������Z�𖳌��ɂ��܂�
	*/
	virtual void BodyDisable() final;
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
class DynamicBox final : public RigidBody
{
private:
	Mesh mesh;
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);
public:
	/*!
	* @brief �����̂𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (scale)	 ���̂̂��ꂼ��̎���1�ӂ̑傫��
	* @param (totalMass) ����
	*/
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
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
class DynamicSphere final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal& radius, const dReal totalMass);
public:
	/*!
	* @brief ���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (radius)    ���a
	* @param (totalMass) ����
	*/
	DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	
	void Draw(Texture& tex) override;
};

/*! 
*	@class DynamicCylinder
*   @brief �~���𐶐����܂�
*/
class DynamicCylinder final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal totalmass, const int direction, const dReal radius, const dReal length);
public:
	/*!
	* @brief �~���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (totalMass) ����
	* @param (direction) �~���̌���
	* @param (radius)    ���a
	* @param (length)    ����
	*/
	DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCylinder(const DynamicCylinder& cylinder);
	DynamicCylinder();
	~DynamicCylinder();

	void Draw(Texture& tex) override;
};
 
/*!
*	@class DynamicCapsule
*   @brief �J�v�Z���𐶐����܂�
*/
class DynamicCapsule final : public RigidBody
{
private:
	void Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
public:
	/*!
	* @brief �J�v�Z���𐶐����܂�
	* @param (pos)       ���̂̒��S���W
	* @param (totalMass) ����
	* @param (direction) �J�v�Z���̌���
	* @param (radius)    ���a
	* @param (length)    ����
	*/
	DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCapsule(const DynamicCapsule& capsule);
	DynamicCapsule();
	~DynamicCapsule();

	void Draw(Texture& tex) override;
};
