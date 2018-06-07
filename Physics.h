#pragma once
#pragma warning (disable : 4819)	//���j�R�[�h
#include <ode/ode.h>
#include <memory>
#include <vector>
#include "Utility.hpp"
#include "RigidBody.h"
#include "Geometry.h"

#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//�}���`�X���b�hDLL
#endif

class DynamicBox;
class DynamicSphere;
class DynamicCylinder;
class DynamicCapsule;
class StaticBox;
class StaticSphere;
class StaticCylinder;
class StaticCapsule;

class Physics
{
private:
	dWorldID world;
	// �Փˊ֌W(�X�y�[�X�ƏՓ˓_�O���[�v)
	dSpaceID collSpace;
	dJointGroupID contactGroup;
	void Create();
	
public:
	Physics();
	~Physics();
	dWorldID GetWorld() const;
	dSpaceID GetCollsionSpace() const;
	dJointGroupID GetContactGroup() const;
	void SetGravity(const Vec3_d& gravity);
    void WorldStep(const float stepTime);

};


class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);

	//Memo::�Փˌ��m�p�̉������o--
	static dGeomID isHitGround;
	void GetHit();
	static bool isHit;
	//--------------------------------

public:
	PhysicsWorld() {};
	~PhysicsWorld() {};
	std::vector<std::unique_ptr<DynamicBox>> pDynamicBox;
	std::vector<std::unique_ptr<DynamicSphere>> pDynamicSphere;
	std::vector<std::unique_ptr<DynamicCapsule>> pDynamicCapsule;
	std::vector<std::unique_ptr<DynamicCylinder>> pDynamicCylinder;
	std::vector<std::unique_ptr<StaticBox>> pStaticBox;
	//�I�u�W�F�N�g�̃Z�b�g�A�b�v���s��
	void AddDynamicSphere(const Vec3& pos, const dReal& r, dReal mass);
	void AddDynamicBox(const Vec3& pos,const Vec3& scale,const dReal mass);
	void AddDynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	void AddDynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	void AddStaticBox(const Vec3& scale);
	//���[���h���X�V
	void UpDate();
	void SetGravity(const Vec3_d& gravity);
};
