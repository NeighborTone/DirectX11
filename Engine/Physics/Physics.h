#pragma once
#pragma warning (disable : 4819)	//ユニコード
#include <ode/ode.h>
#include <memory>
#include <vector>
#include "../Utilitys/Vec.hpp"
#include "RigidBody.h"
#include "Geometry.h"

#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//マルチスレッドDLL
#endif

class RigidBody;
class Geometry;
class DynamicBox;
class DynamicSphere;
class DynamicCylinder;
class DynamicCapsule;
class StaticBox;
class StaticSphere;
class StaticCylinder;
class StaticCapsule;

class Physics
{
private:
	dWorldID world;
	// 衝突関係(スペースと衝突点グループ)
	dSpaceID collSpace;
	dJointGroupID contactGroup;
	void Create();
	
public:
	Physics();
	~Physics();
	dWorldID GetWorld() const;
	dSpaceID GetCollsionSpace() const;
	dJointGroupID GetContactGroup() const;
	void SetGravity(const Vec3_d& gravity);
    void WorldStep(const float stepTime);

};

//シリンダーとカプセルのlengthに平行な軸
enum CylinderDir
{
	X = 1,
	Y = 2,
	Z = 3,
};

class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);
	dGeomID g1;
	dGeomID g2;
	int id1;
	int id2;
public:
	
	PhysicsWorld() {};
	~PhysicsWorld() {};

	std::vector<std::unique_ptr<RigidBody>> pRigidBody;
	std::vector<std::unique_ptr<Geometry>> pGeometry;

	//オブジェクトのセットアップを行う
	int AddRigidBody(RigidBody* pBody);
	int AddGeometry(Geometry* pGeom);
	//いったんvoid
	void IsHitGeom(int ID1, int ID2d);
	//ワールドを更新
	void UpDate();
	void SetGravity(const Vec3_d& gravity);
};

