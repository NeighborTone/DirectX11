#pragma once
#include "Physics.h"
#include "Utility.hpp"
#include "Mesh.h"

class Geometry
{
protected:
	Geometry();
	virtual ~Geometry();
public:
	
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

