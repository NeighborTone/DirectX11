#pragma once
#pragma warning (disable : 4819)	//ユニコード
#include <ode/ode.h>
#include <memory>
#include <vector>
#include "Utility.hpp"
#include "RigidBody.h"

#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//マルチスレッドDLL
#endif

class DynamicBox;
class DynamicSphere;

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


class StaticBox
{
private:
	dGeomID geom;
public:
	StaticBox(const Vec3& scale);
	~StaticBox();
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);
	dGeomID GetGeomID();
};

class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);

	//Memo::衝突検知用の仮メンバ--
	static dGeomID isHitGround;
	void GetHit();
	static bool isHit;
	//--------------------------------

public:
	PhysicsWorld() {};
	~PhysicsWorld() {};
	std::vector<std::unique_ptr<DynamicBox>> pDynamicBox;
	std::vector<std::unique_ptr<DynamicSphere>> pDynamicSphere;
	std::vector<std::unique_ptr<StaticBox>> pStaticBox;
	//オブジェクトのセットアップを行う
	void AddDynamicSphere(const Vec3& pos, const dReal& r, dReal mass);
	void AddDynamicBox(const Vec3& pos,const Vec3& scale,const dReal mass);
	void AddStaticBox(const Vec3& scale);
	//ワールドを更新
	void UpDate();
	void SetGravity(const Vec3_d& gravity);
};
