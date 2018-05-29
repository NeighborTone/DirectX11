#pragma once
#pragma warning (disable : 4819)	//ユニコード
#pragma warning (disable : 4099)	//pdbが無いため
#include <ode/ode.h>
#include "Engine.h"
#include <memory>
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

class Box
{
private:
	dBodyID body;		//剛体
	dGeomID geom;		//衝突検知

	void Create(const Vec3& size, dReal totalMass);
public:
	Box(const Vec3& size, dReal totalMass);
	~Box();
	Vec3 GetPosition() const;
};
class Ground
{
private:
	dGeomID geom;
public:
	Ground(const Vec3& size);
	~Ground();
};
class EntityWorld final
{
private:
	
	static void NearCallback(void *data, dGeomID o1, dGeomID o2);
public:
	std::unique_ptr<Box> pBox;
	std::unique_ptr<Ground> pGround;
	// オブジェクトのセットアップを行う
	void SetupWorld();
	// ワールドを更新
	void UpDate();

};
