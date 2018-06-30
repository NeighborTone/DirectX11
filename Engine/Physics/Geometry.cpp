#include "Geometry.h"
#include "../Engine.h"


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

dGeomID Geometry::GetGeomID() const
{
	return geom;
}

Vec3 Geometry::GetPosition() const
{
	const dReal* values = dGeomGetPosition(geom);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void Geometry::SetPosition(const Vec3& pos)
{
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

void Geometry::SetRotation(const Vec3& angle)
{
	//行列に変換して姿勢を決める
	dMatrix3 odeR;
	dRSetIdentity(odeR);

	DirectX::XMMATRIX directR = DirectX::XMMatrixIdentity();

	directR = DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x)) *
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y)) *
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z))
	);

	odeR[0] = directR.r[0].m128_f32[0];
	odeR[1] = directR.r[0].m128_f32[1];
	odeR[2] = directR.r[0].m128_f32[2];
	odeR[3] = directR.r[0].m128_f32[3];
	odeR[4] = directR.r[1].m128_f32[0];
	odeR[5] = directR.r[1].m128_f32[1];
	odeR[6] = directR.r[1].m128_f32[2];
	odeR[7] = directR.r[1].m128_f32[3];
	odeR[8] = directR.r[2].m128_f32[0];
	odeR[9] = directR.r[2].m128_f32[1];
	odeR[10] = directR.r[2].m128_f32[2];
	odeR[11] = directR.r[2].m128_f32[3];

	dGeomSetRotation(geom, odeR);
}

DirectX::XMMATRIX Geometry::GetRotation() const
{
	const dReal* odeR;
	odeR = dGeomGetRotation(geom);

	DirectX::XMMATRIX directR;

	/*4*4に変換する*/
	directR.r[0].m128_f32[0] = (float)odeR[0];
	directR.r[0].m128_f32[1] = (float)odeR[1];
	directR.r[0].m128_f32[2] = (float)odeR[2];
	directR.r[0].m128_f32[3] = (float)odeR[3];
	directR.r[1].m128_f32[0] = (float)odeR[4];
	directR.r[1].m128_f32[1] = (float)odeR[5];
	directR.r[1].m128_f32[2] = (float)odeR[6];
	directR.r[1].m128_f32[3] = (float)odeR[7];
	directR.r[2].m128_f32[0] = (float)odeR[8];
	directR.r[2].m128_f32[1] = (float)odeR[9];
	directR.r[2].m128_f32[2] = (float)odeR[10];
	directR.r[2].m128_f32[3] = (float)odeR[11];
	directR.r[3].m128_f32[0] = 0;
	directR.r[3].m128_f32[1] = 0;
	directR.r[3].m128_f32[2] = 0;
	directR.r[3].m128_f32[3] = 1;

	return directR;
}




StaticBox::StaticBox(const Vec3& pos, const Vec3& scale)
{
	Create(pos, scale);
}

StaticBox::StaticBox()
{
}

void StaticBox::Create(const Vec3& pos, const Vec3& scale)
{
	//ジオメトリの作成
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

StaticBox::~StaticBox()
{

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
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

StaticCylinder::StaticCylinder(const Vec3 & pos, const dReal radius, const dReal length)
{
	Create(pos, radius, length);
}

StaticCylinder::StaticCylinder()
{
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCylinder::~StaticCylinder()
{

}

void StaticCylinder::Create(const Vec3& pos, const dReal radius, const dReal length)
{
	geom = dCreateCylinder(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}

StaticCapsule::StaticCapsule(const Vec3 & pos, const dReal radius, const dReal length)
{
	Create(pos, radius, length);
}

StaticCapsule::StaticCapsule()
{
	dGeomSetPosition(geom, 0, 0, 0);
}

StaticCapsule::~StaticCapsule()
{

}

void StaticCapsule::Create(const Vec3 & pos, const dReal radius, const dReal length)
{
	geom = dCreateCapsule(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetPosition(geom, pos.x, pos.y, pos.z);
}
