#include "Geometry.h"
#include "Engine.h"


Geometry::Geometry():
	geom(nullptr)
{

}


Geometry::~Geometry()
{
	if (geom != nullptr)
	{
		dGeomDestroy(geom);
		geom = nullptr;
	}
}



StaticBox::StaticBox(const Vec3& scale)
{
	Create(scale);
}

StaticBox::StaticBox()
{
}

void StaticBox::Create(const Vec3 & scale)
{
	//ÉWÉIÉÅÉgÉäÇÃçÏê¨
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticBox::~StaticBox()
{
	
}

Vec3 Geometry::GetPosition() const
{
	auto values = dGeomGetPosition(geom);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void Geometry::SetPosition(const Vec3 & pos)
{
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

dGeomID Geometry::GetGeomID()
{
	return geom;
}

StaticSphere::StaticSphere(const Vec3& pos, const dReal& radius)
{
	Create(pos, radius);
}

StaticSphere::StaticSphere()
{
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticSphere::~StaticSphere()
{

}

void StaticSphere::Create(const Vec3& pos, const dReal& radius)
{
	geom = dCreateSphere(Engine::GetPhysics().GetCollsionSpace(), radius);
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCylinder::StaticCylinder(const Vec3 & pos, const int direction, const dReal radius, const dReal length)
{
	Create(pos, direction, radius, length);
}

StaticCylinder::StaticCylinder()
{
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCylinder::~StaticCylinder()
{

}

void StaticCylinder::Create(const Vec3 & pos, const int direction, const dReal radius, const dReal length)
{
	geom = dCreateCylinder(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCapsule::StaticCapsule(const Vec3 & pos, const int direction, const dReal radius, const dReal length)
{
	Create(pos, direction, radius, length);
}

StaticCapsule::StaticCapsule()
{
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCapsule::~StaticCapsule()
{

}

void StaticCapsule::Create(const Vec3 & pos, const int direction, const dReal radius, const dReal length)
{
	geom = dCreateCapsule(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetPosition(geom, 0, 0, 0);
}
