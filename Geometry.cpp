#include "Geometry.h"
#include "Engine.h"


Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}



StaticBox::StaticBox(const Vec3& size)
{
	//ÉWÉIÉÅÉgÉäÇÃçÏê¨
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), size.x, size.y, size.z);
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticBox::~StaticBox()
{
	if (geom != nullptr)
	{
		dGeomDestroy(geom);
		geom = nullptr;
	}
}

Vec3 StaticBox::GetPosition() const
{
	auto values = dGeomGetPosition(geom);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void StaticBox::SetPosition(const Vec3 & pos)
{
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

dGeomID StaticBox::GetGeomID()
{
	return geom;
}