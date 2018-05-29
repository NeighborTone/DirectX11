#pragma once
#pragma warning (disable : 4819)	//���j�R�[�h
#pragma warning (disable : 4099)	//pdb����������
#include <ode/ode.h>
#include "Engine.h"
#include <memory>
#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//�}���`�X���b�hDLL
#endif

class Physics
{
private:
	dWorldID world;
	// �Փˊ֌W(�X�y�[�X�ƏՓ˓_�O���[�v)
	dSpaceID collSpace;
	dJointGroupID contactGroup;
public:
	Physics();
	~Physics();
	dWorldID GetWorld() const;
	dSpaceID GetCollsionSpace() const;
	dJointGroupID GetContactGroup() const;
    void UpDate(float stepTime = 0.01);
};

class Box
{
private:
	dBodyID body;		//����
	dGeomID geom;		//�Փˌ��m

	void Create(const Vec3& size, dReal totalMass);
public:
	Box(const Vec3& size, dReal totalMass);
	~Box();
	Vec3 GetPosition() const;
};
class Ground
{
private:
	dGeomID geom;
public:
	Ground(const Vec3& size);
	~Ground();
};
class EntityWorld final
{
private:
	
	static void NearCallback(void *data, dGeomID o1, dGeomID o2);
public:
	std::unique_ptr<Box> pBox;
	std::unique_ptr<Ground> pGround;
	// �I�u�W�F�N�g�̃Z�b�g�A�b�v���s��
	void SetupWorld();
	// ���[���h���X�V
	void UpDate();

};
