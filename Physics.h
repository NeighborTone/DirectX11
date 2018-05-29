#pragma once
#pragma warning (disable : 4819)	//ユニコード
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "Engine.h"
#include <memory>
#pragma comment(lib,"drawstuffd.lib")
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
    void UpDate(float stepTime = 0.005);
};

class Box
{
private:
	dBodyID body;
	dGeomID geom;
	void Create(const Vec3& size, dReal totalMass);
public:
	Box(const Vec3& size, dReal totalMass);
	~Box()
	{
		dGeomDestroy(this->geom);
		dBodyDestroy(this->body);
	}
	Vec3 GetPosition() const;
};
class EntityWorld final
{
private:
	
	static void nearCallback(void *data, dGeomID o1, dGeomID o2);
public:
	std::unique_ptr<Box> pBox;
	// オブジェクトのセットアップを行う
	void setupWorld();
	// ワールドを更新
	void update();

};
