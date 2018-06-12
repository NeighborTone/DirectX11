#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"

class Geometry
{
protected:
	dGeomID geom;
	
public:
	Geometry();
	virtual ~Geometry();
	Vec3 GetPosition() const;
	void SetPosition(const Vec3& pos);
	dGeomID GetGeomID();
};


class StaticBox : public Geometry
{
public:
	StaticBox(const Vec3& pos, const Vec3& scale);
	StaticBox();
	void Create(const Vec3& pos, const Vec3& scale);
	~StaticBox();
	
};

class StaticSphere : public Geometry
{
public:
	StaticSphere(const Vec3& pos, const dReal& radius);
	StaticSphere();
	~StaticSphere();
	void Create(const Vec3& pos, const dReal& radius);
};

class StaticCylinder : public Geometry
{
public:
	StaticCylinder(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCylinder();
	~StaticCylinder();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};

class StaticCapsule : public Geometry
{
public:
	StaticCapsule(const Vec3& pos, const int direction, const dReal radius, const dReal length);
	StaticCapsule();
	~StaticCapsule();
	void Create(const Vec3& pos, const int direction, const dReal radius, const dReal length);
};