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
	virtual dGeomID GetGeomID() const final;
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
	* @brief �`��̎p����ݒ肵�܂�
	* @param (angle) �x��(degree)�ł̉�]�l
	*/
	virtual void SetRotation(const Vec3& angle) final;
	/*!
	* @brief �`��̎p������]�s��œ��܂�
	* @return DirectX::XMMATRIX
	*/
	virtual DirectX::XMMATRIX GetRotation() const final;
	
};


class StaticBox final : public Geometry
{
private:
	void Create(const Vec3& pos, const Vec3& scale);
public:
	StaticBox(const Vec3& pos, const Vec3& scale);
	StaticBox();
	~StaticBox();
};

class StaticSphere final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal& radius);
public:
	StaticSphere(const Vec3& pos, const dReal& radius);
	StaticSphere();
	~StaticSphere();	
};

class StaticCylinder final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal radius, const dReal length);
public:
	StaticCylinder(const Vec3& pos, const dReal radius, const dReal length);
	StaticCylinder();
	~StaticCylinder();
};

class StaticCapsule final : public Geometry
{
private:
	void Create(const Vec3& pos, const dReal radius, const dReal length);
public:
	StaticCapsule(const Vec3& pos, const dReal radius, const dReal length);
	StaticCapsule();
	~StaticCapsule();
};