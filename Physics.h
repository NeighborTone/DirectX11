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

class RigidBody;
class Geometry;
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

//�V�����_�[�ƃJ�v�Z����length�ɕ��s�Ȏ�
enum CylinderDir
{
	X = 1,
	Y = 2,
	Z = 3,
};

class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);

public:
	
	PhysicsWorld() {};
	~PhysicsWorld() {};

	std::vector<std::unique_ptr<RigidBody>> pRigidBody;
	std::vector<std::unique_ptr<Geometry>> pGeometry;

	//�I�u�W�F�N�g�̃Z�b�g�A�b�v���s��
	void AddRigidBody(RigidBody* pBody);
	void AddGeometry(Geometry* pGeom);

	//���[���h���X�V
	void UpDate();
	void SetGravity(const Vec3_d& gravity);
};

