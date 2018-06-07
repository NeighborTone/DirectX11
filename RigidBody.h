#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"
//とりあえずテンプレートメソッドパターンで実装

class RigidBody
{
protected:
	dBodyID body;		//剛体
	dGeomID geom;		//衝突検知
	Vec3 pos;
	RigidBody();
	virtual ~RigidBody();
public:
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);
	void AddForce(const Vec3& force);
	void AddVelocity(const Vec3& velocity);
	Vec3 GetVelocity();
	void SetQuaternion(Vec3& angle);
	Vec3 GetQuaternion();
	void SetRotation(Vec3& angle);
	Vec3 GetRotation();
	void BodyEnable();
	void BodyDisable();
	virtual void Draw(Texture& tex) = 0;
};

class DynamicBox : public RigidBody
{
private:
	Mesh mesh;
public:
	DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass);
	DynamicBox(const DynamicBox& box);
	DynamicBox();
	~DynamicBox();
	void Create(const Vec3& pos, const Vec3& scale, dReal totalMass);
	void Draw(Texture& tex) override;

	//アライメント対策
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

class DynamicSphere : public RigidBody
{
public:
	DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	void Create(const Vec3& pos, const dReal& r, const dReal totalMass);
	void Draw(Texture& tex) override;
};

class DynamicCylinder : public RigidBody
{
public:
	DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCylinder(const DynamicCylinder& cylinder);
	DynamicCylinder();
	~DynamicCylinder();
	void Create(const Vec3& pos , const dReal total_mass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
 
class DynamicCapsule : public RigidBody
{
public:
	DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	DynamicCapsule(const DynamicCapsule& capsule);
	DynamicCapsule();
	~DynamicCapsule();
	void Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length);
	void Draw(Texture& tex) override;
};
