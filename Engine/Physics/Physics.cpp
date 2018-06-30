#include "Physics.h"
#include "../Engine.h"
#include <iostream>

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
	dWorldSetContactSurfaceLayer(world, 0.0001);

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

void PhysicsWorld::IsHitGeom(int ID1, int ID2)
{
	//このままだと判定1個分（最後に呼んだやつ）しか取れない
	g1 = pGeometry[ID1]->GetGeomID();
	g2 = pGeometry[ID2]->GetGeomID();
}

void PhysicsWorld::NearCallback(void *data, dGeomID o1, dGeomID o2)
{
	/*void *dataに任意のポインタを持たせられる */
	PhysicsWorld* pPhysics = static_cast<PhysicsWorld*>(data);
	
	dBodyID b1 = dGeomGetBody(o1); // 物体1
	dBodyID b2 = dGeomGetBody(o2); // 物体2
	if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact))
		return; // 衝突対象でない物体の衝突ははずす

	/*衝突時しそうなときにしかこの関数は呼ばれない*/
	//NearCallback(近似的に衝突判定された2つのジオメトリの詳細な当たり判定を行う)
	static constexpr int N = 10;	//接触点数の上限
	dContact contact[N];
	int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));	//nには衝突点数が返る

	
	//---------------こんな感じで取得できた-------------------------------------------------------------
	if (pPhysics->g1 == o1 || pPhysics->g1 == o2)
	{
		if(pPhysics->g2 == o1 || pPhysics->g2 == o2)
		//箱が当たっていればHit!!!!!!!!!!!!
		std::cout << "HogeBox Hit!!!!!!" << std::endl;
	}
	//-----------------------------------------------------------------------------------------------

	//接触点を算出したり、接触点の性質などを設定
	for (int i = 0; i < n; ++i)
	{
		contact[i].surface.mode = dContactBounce;	//接触点にどのような作用を与えるか指定
		contact[i].surface.mu = dInfinity;			//摩擦係数
		contact[i].surface.mu2 = 0.0;
		contact[i].surface.rho = 0.0;
		contact[i].surface.rho2 = 0.0;
		contact[i].surface.rhoN = 0.0;
		contact[i].surface.bounce = 0.25;			//反発係数0~1
		contact[i].surface.bounce_vel = 0.0;		//最低反射速度
		contact[i].surface.slip1 = 0.0;
		contact[i].surface.slip2 = 0.0;
		auto joint = dJointCreateContact(Engine::GetPhysics().GetWorld(), Engine::GetPhysics().GetContactGroup(), &contact[i]);
		dJointAttach(joint, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
	}
}


int PhysicsWorld::AddRigidBody(RigidBody* pBody)
{
	pRigidBody.emplace_back(pBody);
	return pRigidBody.size() - 1;
}

int PhysicsWorld::AddGeometry(Geometry* pGeom)
{
	pGeometry.emplace_back(pGeom);
	return pGeometry.size() - 1;
}

void PhysicsWorld::UpDate()
{
	//必ず物理世界の更新処理の一番始めで呼び出す
	//衝突しそうな２つのジオメトリが発生したら、それらをNearCallback関数に渡す
	dSpaceCollide(Engine::GetPhysics().GetCollsionSpace(), this, &PhysicsWorld::NearCallback);
	Engine::GetPhysics().WorldStep(0.01f);	
	//これを忘れると1ステップ前の接触点がおかしくなる
	dJointGroupEmpty(Engine::GetPhysics().GetContactGroup());	

}

void PhysicsWorld::SetGravity(const Vec3_d& gravity)
{
	Engine::GetPhysics().SetGravity(gravity);
}

