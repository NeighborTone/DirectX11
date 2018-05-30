#pragma once
#pragma warning (disable : 4819)	//ユニコード
#pragma warning (disable : 4099)	//pdbが無いため
#include <ode/ode.h>
#include <memory>
#include <vector>
#include "Utility.hpp"

#if defined(_DEBUG)
#pragma comment(lib,"oded.lib")
#else
#pragma comment(lib,"ode.lib")		//マルチスレッドDLL
#endif

class Physics
{
private:
	dWorldID world;
	// 衝突関係(スペースと衝突点グループ)
	dSpaceID collSpace;
	dJointGroupID contactGroup;
public:
	Physics();
	~Physics();
	dWorldID GetWorld() const;
	dSpaceID GetCollsionSpace() const;
	dJointGroupID GetContactGroup() const;
    void UpDate(float stepTime = 0.01);
};

class DynamicBox
{
private:
	dBodyID body;		//剛体
	dGeomID geom;		//衝突検知

	void Create(const Vec3& scale, dReal totalMass);
public:
	DynamicBox(const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	~DynamicBox();
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);
};
class StaticBox
{
private:
	dGeomID geom;
public:
	StaticBox(const Vec3& size);
	~StaticBox();
};
class PhysicsWorld final
{
private:
	static void NearCallback(void *data, dGeomID o1, dGeomID o2);
public:
	PhysicsWorld() {};
	~PhysicsWorld() {};
	std::vector<std::unique_ptr<DynamicBox>> pDynamicBox;
	std::vector<std::unique_ptr<StaticBox>> pStaticBox;
	// オブジェクトのセットアップを行う
	void AddDynamicBox(Vec3& scale, dReal mass);
	void AddStaticBox(Vec3& scale);
	// ワールドを更新
	void UpDate();

};
