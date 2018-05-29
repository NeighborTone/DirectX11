#include "Physics.h"


Physics::Physics()
{
	Engine::COMInitialize();
	//ODEの初期化
	dInitODE();
	//ワールドの作成
	world = dWorldCreate();
	//重力の設定
	dWorldSetGravity(world,0,-9.81,0);
	collSpace = dHashSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
}

Physics::~Physics()
{
	//ワールドの削除
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
	// ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, size.x, size.y, size.z);
	dBodySetMass(body, &mass);

	// ジオメトリを作成してボディをアタッチ
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
	dGeomSetBody(geom, body);

	//ポジション
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
	// ジオメトリの作成
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
}

Ground::~Ground()
{
	dGeomDestroy(geom);
}


void EntityWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	// nearCallback(近似的に衝突判定された2つのジオメトリの詳細な当たり判定を行う)
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
	Engine::GetPhysics().UpDate();	//ここで呼ばないと重力がおかしくなる
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());
}
