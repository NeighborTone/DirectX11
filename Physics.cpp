#include "Physics.h"
#include "Engine.h"

Physics::Physics()
{
	Engine::COMInitialize();
	//ODE�̏�����
	dInitODE();
	//���[���h�̍쐬
	world = dWorldCreate();
	//�d�͂̐ݒ�
	dWorldSetGravity(world, 0, -9.81, 0);
	//�Փˌ��o�v�Z
	collSpace = dHashSpaceCreate(0);
	//�ڐG�_�̏W�܂肪�i�[�����
	contactGroup = dJointGroupCreate(0);
}

Physics::~Physics()
{
	//���[���h�̍폜
	dJointGroupDestroy(contactGroup);
	dSpaceDestroy(collSpace);
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

void Physics::WorldStep(const float stepTime)
{
	dWorldStep(world, stepTime);
}

void DynamicBox::Create(const Vec3& pos, const Vec3& scale, dReal totalMass)
{
	//�{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, scale.x, scale.y, scale.z);
	dBodySetMass(body, &mass);

	//�W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetBody(geom, body);
	this->pos = pos;
	//�|�W�V����
	dBodySetPosition(body, this->pos.x, this->pos.y, this->pos.z);
}

DynamicBox::DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass)
{
	this->pos = pos;
	Create(this->pos, scale, totalMass);
}

DynamicBox::DynamicBox(const DynamicBox& box)
{
	body = box.body;
	geom = box.geom;
}

DynamicBox::DynamicBox():
	body(nullptr),
	geom(nullptr),
	pos(0,0,0)
{
	dBodySetPosition(body,0,0,0);
}

DynamicBox::~DynamicBox()
{
	if (geom != nullptr)
	{
		dGeomDestroy(geom);
		geom = nullptr;
	}
	if (body != nullptr)
	{
		dBodyDestroy(body);
		body = nullptr;
	}
}

Vec3 DynamicBox::GetPosition() const
{
	auto values = dBodyGetPosition(body);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void DynamicBox::SetPosition(const Vec3& pos)
{
	this->pos = pos;
	dBodySetPosition(body, this->pos.x, this->pos.y, this->pos.z);
}


StaticBox::StaticBox(const Vec3& size)
{
	//�W�I���g���̍쐬
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticBox::~StaticBox()
{
	if (geom != nullptr)
	{
		dGeomDestroy(geom);
		geom = nullptr;
	}
}

Vec3 StaticBox::GetPosition() const
{
	auto values = dGeomGetPosition(geom);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void StaticBox::SetPosition(const Vec3 & pos)
{
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

void PhysicsWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	//NearCallback(�ߎ��I�ɏՓ˔��肳�ꂽ2�̃W�I���g���̏ڍׂȓ����蔻����s��)
	static const int N = 20;		//�ڐG�_���̏��
	dContact contact[N];
	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));	//n�ɂ͏Փ˓_�����Ԃ�
	//�ڐG�_���Z�o������A�ڐG�_�̐����Ȃǂ�ݒ�
	for (int i = 0; i < n; i++)
	{
		contact[i].surface.mode = dContactBounce;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0.0;
		contact[i].surface.rho = 0.0;
		contact[i].surface.rho2 = 0.0;
		contact[i].surface.rhoN = 0.0;
		contact[i].surface.bounce = 0.25;	//0~1
		contact[i].surface.bounce_vel = 0.0;
		contact[i].surface.slip1 = 0.0;
		contact[i].surface.slip2 = 0.0;
		auto joint = dJointCreateContact(Engine::GetPhysics().GetWorld(), Engine::GetPhysics().GetContactGroup(), &contact[i]);
		dJointAttach(joint, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
	}
}

void PhysicsWorld::AddDynamicBox(const Vec3& pos,const Vec3& scale, const dReal mass)
{
	pDynamicBox.emplace_back(std::make_unique<DynamicBox>(pos, scale, mass));
}

void PhysicsWorld::AddStaticBox(const Vec3& scale)
{
	pStaticBox.emplace_back(std::make_unique<StaticBox>(scale));
}

void PhysicsWorld::UpDate()
{
	//�K���������E�̍X�V�����̈�Ԏn�߂ŌĂяo��
	//�Փ˂������ȂQ�̃W�I���g��������������A������NearCallback�֐��ɓn��
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &PhysicsWorld::NearCallback);	
	Engine::GetPhysics().WorldStep(0.01f);	
	//�����Y����1�X�e�b�v�O�̐ڐG�_�����������Ȃ�
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());	
}

