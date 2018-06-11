#pragma once
#pragma warning (disable : 4819)	//ユニコード
#include <ode/ode.h>
#include <memory>
#include <vector>
#include "Utility.hpp"
#include "RigidBody.h"
#include "Geometry.h"

#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//マルチスレッドDLL
#endif

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
enum  CylinderDir
{
	X = 1,
	Y = 2,
	Z = 3,
};

class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);


public:
	
	PhysicsWorld() {};
	~PhysicsWorld() {};
	std::vector<std::unique_ptr<DynamicBox>> pDynamicBox;
	std::vector<std::unique_ptr<DynamicSphere>> pDynamicSphere;
	std::vector<std::unique_ptr<DynamicCapsule>> pDynamicCapsule;
	std::vector<std::unique_ptr<DynamicCylinder>> pDynamicCylinder;
	std::vector<std::unique_ptr<StaticBox>> pStaticBox;
	std::vector<std::unique_ptr<StaticSphere>> pStaticSphere;
	std::vector<std::unique_ptr<StaticCapsule>> pStaticCapsule;
	std::vector<std::unique_ptr<StaticCylinder>> pStaticCylinder;
	//オブジェクトのセットアップを行う
	void AddDynamicBox(const Vec3& pos,const Vec3& scale,const dReal mass);
	void AddDynamicSphere(const Vec3& pos, const dReal& r, dReal mass);
	void AddDynamicCapsule(const Vec3& pos, const dReal totalMass, const CylinderDir direction, const dReal radius, const dReal length);
	void AddDynamicCylinder(const Vec3& pos, const dReal totalMass, const CylinderDir direction, const dReal radius, const dReal length);
	void AddStaticBox(const Vec3& pos, const Vec3& scale);
	void AddStaticSphere(const Vec3& pos, const dReal& radius);
	void AddStaticCapsule(const Vec3& pos, const CylinderDir direction, const dReal radius, const dReal length);
	void AddStaticCylinder(const Vec3& pos, const CylinderDir direction, const dReal radius, const dReal length);
	//ワールドを更新
	void UpDate();
	void SetGravity(const Vec3_d& gravity);
};

