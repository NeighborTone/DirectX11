#pragma once
#pragma warning (disable : 4819)	//���j�R�[�h
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "Engine.h"
#include <memory>
#pragma comment(lib,"drawstuffd.lib")
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
    void UpDate(float stepTime = 0.005);
};

class Box
{
private:
	dBodyID body;
	dGeomID geom;
	void Create(const Vec3& size, dReal totalMass);
public:
	Box(const Vec3& size, dReal totalMass);
	~Box()
	{
		dGeomDestroy(this->geom);
		dBodyDestroy(this->body);
	}
	Vec3 GetPosition() const;
};
class EntityWorld final
{
private:
	
	static void nearCallback(void *data, dGeomID o1, dGeomID o2);
public:
	std::unique_ptr<Box> pBox;
	// �I�u�W�F�N�g�̃Z�b�g�A�b�v���s��
	void setupWorld();
	// ���[���h���X�V
	void update();

};
