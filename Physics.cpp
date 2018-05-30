#include "Physics.h"
#include "Engine.h"

Physics::Physics()
{
	Engine::COMInitialize();
	//ODEの初期化
	dInitODE();
	//ワールドの作成
	world = dWorldCreate();
	//重力の設定
	dWorldSetGravity(world, 0, -9.81, 0);
	//衝突検出計算
	collSpace = dHashSpaceCreate(0);
	//接触点の集まりが格納される
	contactGroup = dJointGroupCreate(0);
}

Physics::~Physics()
{
	//ワールドの削除
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
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, scale.x, scale.y, scale.z);
	dBodySetMass(body, &mass);

	//ジオメトリを作成してボディをアタッチ
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetBody(geom, body);
	this->pos = pos;
	//ポジション
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
	//ジオメトリの作成
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
	//NearCallback(近似的に衝突判定された2つのジオメトリの詳細な当たり判定を行う)
	static const int N = 20;		//接触点数の上限
	dContact contact[N];
	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));	//nには衝突点数が返る
	//接触点を算出したり、接触点の性質などを設定
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
	//必ず物理世界の更新処理の一番始めで呼び出す
	//衝突しそうな２つのジオメトリが発生したら、それらをNearCallback関数に渡す
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &PhysicsWorld::NearCallback);	
	Engine::GetPhysics().WorldStep(0.01f);	
	//これを忘れると1ステップ前の接触点がおかしくなる
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());	
}

