#include "Physics.h"
#include "Engine.h"

Physics::Physics()
{
	Engine::COMInitialize();
	Create();
}

Physics::~Physics()
{
	//���[���h�̍폜
	dJointGroupDestroy(contactGroup);
	dSpaceDestroy(collSpace);
	dWorldDestroy(world);
	dCloseODE();
}

void Physics::Create()
{
	//ODE�̏�����
	dInitODE();
	//���[���h�̍쐬
	world = dWorldCreate();
	//�d�͂̐ݒ�
	dWorldSetGravity(world, 0, -9.81, 0);

	//�S�������̐ݒ�:CFM(constraint force mixing)
	//�ڑ�����Ă���W���C���g�����[�U�����V�~�����[�V�����ɂ�藣��Ă��܂����ꍇ�C 2�ڑ��_���߂Â��悤�ȗ͂𕉉ׂ���D���̂Ƃ��̗͂��R���g���[�����邽�߂̃p�����[�^
	dWorldSetCFM(world, 1e-5);

	//�C�����x(correcting velocity)
	//���̒l��ݒ肷�邱�ƂŁA�ڐG���ɐ[���߂荞�񂾃I�u�W�F�N�g���傫���͂����邱�Ƃ�h�����Ƃ��ł��܂��B����l��dInfinity�ł��B
	dWorldSetContactMaxCorrectingVel(world, dInfinity);

	//�\�ʑw�̌���(the depth of the surface layer)
	//���ׂẴW�I���g���̕\�ʑw�̌�����ݒ肵�܂��B�ڐG�����������Ƃ��ɂ́A�ő�ł��̌����܂ł߂荞�ނ��ƂɂȂ�܂��B
	//����ɏ����Ȓl��ݒ肷�邱�ƂŁA�ڐG������Ԃ��������邱�Ƃɂ��U��(Jitter) ��}���邱�Ƃ��ł��܂��B
	dWorldSetContactSurfaceLayer(world, 0.001);

	//�W���C���g�덷�C���p�����[�^:ERP(Error Reduction Parameter)
	dWorldSetERP(world, 0.8);

	//�Փˌ��o�v�Z
	collSpace = dHashSpaceCreate(0);
	//�ڐG�_�̏W�܂肪�i�[�����
	contactGroup = dJointGroupCreate(0);
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

void Physics::SetGravity(const Vec3_d& gravity)
{
	dWorldSetGravity(world, gravity.x, gravity.y, gravity.z);
}

void Physics::WorldStep(const float stepTime)
{
	dWorldStep(world, stepTime);
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

dGeomID StaticBox::GetGeomID()
{
	return geom;
}
dGeomID PhysicsWorld::isHitGround = nullptr;
bool  PhysicsWorld::isHit = false;
void PhysicsWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	/*�Փˎ��������ȂƂ��ɂ������̊֐��͌Ă΂�Ȃ�*/
	//NearCallback(�ߎ��I�ɏՓ˔��肳�ꂽ2�̃W�I���g���̏ڍׂȓ����蔻����s��)
	static const int N = 20;		//�ڐG�_���̏��
	dContact contact[N];
	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));	//n�ɂ͏Փ˓_�����Ԃ�

	//Memo::������ : ���Ƃ��̑��̃W�I���g���Ƃ̏Փˌ��o�����Ă���----------------
	int ret = ((isHitGround == o1 || isHitGround == o2));
	if (ret)
	{
		isHit = true;
	}
	//---------------------------------------------------------------------------

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

void PhysicsWorld::GetHit()
{
	//Memo::������ : �Ƃ肠�������̃W�I���g�����w��
	isHitGround = pStaticBox[0]->GetGeomID();
}

void PhysicsWorld::AddDynamicSphere(const Vec3 & pos, const dReal & r, dReal mass)
{
	pDynamicSphere.emplace_back(std::make_unique<DynamicSphere>(pos, r, mass));
}

void PhysicsWorld::AddDynamicBox(const Vec3& pos,const Vec3& scale, const dReal mass)
{
	pDynamicBox.emplace_back(std::make_unique<DynamicBox>(pos, scale, mass));
}

void PhysicsWorld::AddDynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	pDynamicCapsule.emplace_back(std::make_unique<DynamicCapsule>(pos, totalMass, direction, radius, length));
}

void PhysicsWorld::AddDynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	pDynamicCylinder.emplace_back(std::make_unique<DynamicCylinder>(pos, totalMass, direction, radius, length));
}

void PhysicsWorld::AddStaticBox(const Vec3& scale)
{
	pStaticBox.emplace_back(std::make_unique<StaticBox>(scale));
	GetHit();
}

void PhysicsWorld::UpDate()
{
	isHit = false;
	//�K���������E�̍X�V�����̈�Ԏn�߂ŌĂяo��
	//�Փ˂������ȂQ�̃W�I���g��������������A������NearCallback�֐��ɓn��
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &PhysicsWorld::NearCallback);	
	Engine::GetPhysics().WorldStep(0.01f);	
	//�����Y����1�X�e�b�v�O�̐ڐG�_�����������Ȃ�
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());	

	//std::cout << std::boolalpha << isHit << std::endl;
}

void PhysicsWorld::SetGravity(const Vec3_d& gravity)
{
	Engine::GetPhysics().SetGravity(gravity);
}

