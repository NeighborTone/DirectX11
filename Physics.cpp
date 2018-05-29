#include "Physics.h"


Physics::Physics()
{
	Engine::COMInitialize();
	//ODE�̏�����
	dInitODE();
	//���[���h�̍쐬
	world = dWorldCreate();
	//�d�͂̐ݒ�
	dWorldSetGravity(world,0,-9.81,0);
	collSpace = dHashSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
}

Physics::~Physics()
{
	//���[���h�̍폜
	dWorldDestroy(world);
	dCloseODE();

}

dWorldID Physics::GetWorld() const
{
	return world;
}

dSpaceID Physics::GetCollsionSpace() const
{
	return collSpace;
}

dJointGroupID Physics::GetContactGroup() const
{
	return contactGroup;
}

void Physics::UpDate(float stepTime)
{

	dWorldStep(world, stepTime);

}

void EntityWorld::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	// nearCallback(�ߎ��I�ɏՓ˔��肳�ꂽ2�̃W�I���g���̏ڍׂȓ����蔻����s��)
	int i, n;

	const int N = 10;
	dContact contact[N];
	n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
	if (n > 0) {
		for (i = 0; i<n; i++) {
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
				dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.bounce = 0.25;
			contact[i].surface.bounce_vel = 0.0;
			contact[i].surface.slip1 = 0.1;
			contact[i].surface.slip2 = 0.1;
			contact[i].surface.soft_erp = 0.5;
			contact[i].surface.soft_cfm = 0.3;
			dJointID c = dJointCreateContact(Engine::GetPhysics().GetWorld(), Engine::GetPhysics().GetContactGroup(), &contact[i]);
			dJointAttach(c,
				dGeomGetBody(contact[i].geom.g1),
				dGeomGetBody(contact[i].geom.g2));
		}
	}
}

void EntityWorld::setupWorld()
{
	this->pBox = std::make_unique<Box>(Vec3(1.0, 1.0, 1.0), 1);
}

void EntityWorld::update()
{
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &EntityWorld::nearCallback);
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());
}

void Box::Create(const Vec3 & size, dReal totalMass)
{
	// �{�f�B������Ď��ʂ�ݒ肷��
	this->body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, size.x, size.y, size.z);
	dBodySetMass(this->body, &mass);

	// �W�I���g�����쐬���ă{�f�B���A�^�b�`
	this->geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
	dGeomSetBody(this->geom, this->body);
}

Box::Box(const Vec3& size, dReal totalMass)
{
	Create(size, totalMass);
}

Vec3 Box::GetPosition() const
{
	auto values = dBodyGetPosition(this->body);
	return Vec3(values[0], values[1], values[2]);
}
