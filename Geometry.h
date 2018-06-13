#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"

/*!
*	@class Geometry
*   @brief  OpenDynamicsEngine�ŏՓ˔�����s�������`��𐶐�����N���X�̃C���^�[�t�F�[�X�ł�
*   @detail ���̃N���X��PhysicsWorld��AddGeometry���\�b�h�̈����Ő������܂�
*	���̃N���X�P�̂ł͎g�p�ł��܂���
*	���̃N���X�������\�b�h�̏������e�͌p����ł��ς��܂���
*/
class Geometry
{
protected:
	dGeomID geom;
	
public:
	Geometry();
	virtual ~Geometry();
	/*!
	* @brief dGeomID���擾���܂�
	* @return dGeomID
	*/
	dGeomID GetGeomID() const;
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
	* @brief �`��̎p����ݒ肵�܂�
	* @param (angle) �x��(degree)�ł̉�]�l
	*/
	void SetRotation(const Vec3& angle);
	/*!
	* @brief �`��̎p������]�s��œ��܂�
	* @return DirectX::XMMATRIX
	*/
	DirectX::XMMATRIX GetRotation() const;
	
};


class StaticBox : public Geometry
{
public:
	StaticBox(const Vec3& pos, const Vec3& scale);
	StaticBox();
	void Create(const Vec3& pos, const Vec3& scale);
	~StaticBox();
	
};

class StaticSphere : public Geometry
{
public:
	StaticSphere(const Vec3& pos, const dReal& radius);
	StaticSphere();
	~StaticSphere();
	void Create(const Vec3& pos, const dReal& radius);
};

class StaticCylinder : public Geometry
{
public:
	StaticCylinder(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCylinder();
	~StaticCylinder();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};

class StaticCapsule : public Geometry
{
public:
	StaticCapsule(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCapsule();
	~StaticCapsule();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};