#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"
class RigidBody
{
protected:
	dBodyID body;		//çÑëÃ
	dGeomID geom;		//è’ìÀåüím
	Vec3 pos;
	RigidBody();
	virtual ~RigidBody();
public:
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);
	void AddForce(const Vec3& force);
	void AddVelocity(const Vec3& velocity);
	Vec3 GetVelocity();
	void SetAngle(Vec3& angle);
	Vec3 GetAngle();
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
	DynamicSphere(const Vec3& pos, const dReal& r,dReal totalMass);
	DynamicSphere(const DynamicSphere& sphere);
	DynamicSphere();
	~DynamicSphere();
	void Create(const Vec3& pos, const dReal& r, dReal totalMass);
	void Draw(Texture& tex) override;
};