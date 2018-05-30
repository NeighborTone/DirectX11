#pragma once
#pragma warning (disable : 4819)	//ユニコード
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
    void WorldStep(const float stepTime);

};

class DynamicBox
{
private:
	dBodyID body;		//剛体
	dGeomID geom;		//衝突検知
	Vec3 pos;
public:
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);

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

};

class PhysicsWorld final
{
private:
	static void NearCallback(void* data, dGeomID o1, dGeomID o2);
public:
	PhysicsWorld() {};
	~PhysicsWorld() {};
	std::vector<std::unique_ptr<DynamicBox>> pDynamicBox;
	std::vector<std::unique_ptr<StaticBox>> pStaticBox;
	// オブジェクトのセットアップを行う
	void AddDynamicBox(const Vec3& pos,const Vec3& scale,const dReal mass);
	void AddStaticBox(const Vec3& scale);
	// ワールドを更新
	void UpDate();

};
