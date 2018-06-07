#include "Physics.h"
#include "Engine.h"

Physics::Physics()
{
	Engine::COMInitialize();
	Create();
}

Physics::~Physics()
{
	//ワールドの削除
	dJointGroupDestroy(contactGroup);
	dSpaceDestroy(collSpace);
	dWorldDestroy(world);
	dCloseODE();
}

void Physics::Create()
{
	//ODEの初期化
	dInitODE();
	//ワールドの作成
	world = dWorldCreate();
	//重力の設定
	dWorldSetGravity(world, 0, -9.81, 0);

	//拘束条件の設定:CFM(constraint force mixing)
	//接続されているジョイントがユーザ操作やシミュレーションにより離れてしまった場合， 2接続点が近づくような力を負荷する．このときの力をコントロールするためのパラメータ
	dWorldSetCFM(world, 1e-5);

	//修正速度(correcting velocity)
	//この値を設定することで、接触時に深くめり込んだオブジェクトが大きくはじけることを防ぐことができます。既定値はdInfinityです。
	dWorldSetContactMaxCorrectingVel(world, dInfinity);

	//表面層の厚さ(the depth of the surface layer)
	//すべてのジオメトリの表面層の厚さを設定します。接触が発生したときには、最大でこの厚さまでめり込むことになります。
	//これに小さな値を設定することで、接触がくり返し発生することによる振動(Jitter) を抑えることができます。
	dWorldSetContactSurfaceLayer(world, 0.001);

	//ジョイント誤差修正パラメータ:ERP(Error Reduction Parameter)
	dWorldSetERP(world, 0.8);

	//衝突検出計算
	collSpace = dHashSpaceCreate(0);
	//接触点の集まりが格納される
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

dGeomID StaticBox::GetGeomID()
{
	return geom;
}
dGeomID PhysicsWorld::isHitGround = nullptr;
bool  PhysicsWorld::isHit = false;
void PhysicsWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	/*衝突時しそうなときにしかこの関数は呼ばれない*/
	//NearCallback(近似的に衝突判定された2つのジオメトリの詳細な当たり判定を行う)
	static const int N = 20;		//接触点数の上限
	dContact contact[N];
	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));	//nには衝突点数が返る

	//Memo::仮処理 : 床とその他のジオメトリとの衝突検出をしている----------------
	int ret = ((isHitGround == o1 || isHitGround == o2));
	if (ret)
	{
		isHit = true;
	}
	//---------------------------------------------------------------------------

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

void PhysicsWorld::GetHit()
{
	//Memo::仮処理 : とりあえず床のジオメトリを指定
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
	//必ず物理世界の更新処理の一番始めで呼び出す
	//衝突しそうな２つのジオメトリが発生したら、それらをNearCallback関数に渡す
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), nullptr, &PhysicsWorld::NearCallback);	
	Engine::GetPhysics().WorldStep(0.01f);	
	//これを忘れると1ステップ前の接触点がおかしくなる
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());	

	//std::cout << std::boolalpha << isHit << std::endl;
}

void PhysicsWorld::SetGravity(const Vec3_d& gravity)
{
	Engine::GetPhysics().SetGravity(gravity);
}

