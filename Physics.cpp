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


void Box::Create(const Vec3 & size, dReal totalMass)
{
	// �{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, size.x, size.y, size.z);
	dBodySetMass(body, &mass);

	// �W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
	dGeomSetBody(geom, body);

	//�|�W�V����
	dBodySetPosition(body, 0, 10, 0);
}

Box::Box(const Vec3& size, dReal totalMass)
{
	Create(size, totalMass);
}

Box::~Box()
{
	dGeomDestroy(geom);
	dBodyDestroy(body);
}

Vec3 Box::GetPosition() const
{
	auto values = dBodyGetPosition(body);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

Ground::Ground(const Vec3& size)
{
	// �W�I���g���̍쐬
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
}

Ground::~Ground()
{
	dGeomDestroy(geom);
}


void EntityWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	// nearCallback(�ߎ��I�ɏՓ˔��肳�ꂽ2�̃W�I���g���̏ڍׂȓ����蔻����s��)
	const int N = 10;
	dContact contact[N];
	

	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
	for (int i = 0; i < n; i++)
	{
		contact[i].surface.mode = dContactBounce;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.bounce = 0.35;	//0~1
		contact[i].surface.bounce_vel = 0.0;
		auto joint = dJointCreateContact(Engine::GetPhysics().GetWorld(), Engine::GetPhysics().GetContactGroup(), &contact[i]);
		dJointAttach(joint, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
	}
}

void EntityWorld::SetupWorld()
{
	pBox = std::make_unique<Box>(Vec3(1.0, 1.0, 1.0), 5);
	pGround = std::make_unique<Ground>(Vec3(10, 1, 10));
}

void EntityWorld::UpDate()
{
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &EntityWorld::NearCallback);
	Engine::GetPhysics().UpDate();	//�����ŌĂ΂Ȃ��Əd�͂����������Ȃ�
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());
}
