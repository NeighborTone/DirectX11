#include "RigidBody.h"
#include "../Engine.h"
#include <SimpleMath.h>

RigidBody::RigidBody() :
	body(nullptr),
	geom(nullptr)
{
}
RigidBody::~RigidBody()
{
	if (geom != nullptr)
	{
		dGeomDestroy(geom);
		geom = nullptr;
	}
	if (body != nullptr)
	{
		dBodyDestroy(body);
		body = nullptr;
	}
}
dGeomID RigidBody::GetGeomID() const
{
	return geom;
}
dBodyID RigidBody::GetBodyID() const
{
	return body;
}
Vec3 RigidBody::GetPosition() const
{
	const dReal* values = dBodyGetPosition(body);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void RigidBody::SetPosition(const Vec3& pos)
{
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void RigidBody::AddForce(const Vec3& force)
{
	//Memo::これを利用することで、剛体ごとに重力の値を設定できる
	dBodyAddForce(body, force.x, force.y, force.z);
}

void RigidBody::AddVelocity(const Vec3 & velocity)
{
	dBodySetLinearVel(body, velocity.x, velocity.y, velocity.z);
}

Vec3 RigidBody::GetVelocity() const
{
	const dReal* vel = dBodyGetLinearVel(body);
	return Vec3((float)vel[0], (float)vel[1], (float)vel[2]);
}

void RigidBody::SetQuaternion(Vec3& angle)
{
	//まだなので適当
	dQuaternion q = 
	{  1,
		DirectX::XMConvertToRadians(angle.x),
		DirectX::XMConvertToRadians(angle.y),
		DirectX::XMConvertToRadians(angle.z) 
	};

	dBodySetQuaternion(body, q);

}

Vec3 RigidBody::GetQuaternion()
{
	//まだなので適当
	const dReal* val = dBodyGetQuaternion(body);

	return Vec3(
		DirectX::XMConvertToDegrees((float)val[1]), 
		DirectX::XMConvertToDegrees((float)val[2]), 
		DirectX::XMConvertToDegrees((float)val[3])
	);
}

void RigidBody::SetAxisAndAngle(Axis axis, float degree)
{
	//一つの角度しか決められないので用途不明だが一応用意しておいた
	dMatrix3 R;
	if (axis == Axis::X)
	{
		dRFromAxisAndAngle(R, 1.0, 0.0, 0.0, DirectX::XMConvertToRadians(degree));	//x軸周りに[rad]回転
	}
	if (axis == Axis::Y)
	{
		dRFromAxisAndAngle(R, 0.0, 1.0, 0.0, DirectX::XMConvertToRadians(degree));	//y軸周りに[rad]回転
	}
	if (axis == Axis::Z)
	{
		dRFromAxisAndAngle(R, 0.0, 0.0, 1.0, DirectX::XMConvertToRadians(degree));	//z軸周りに[rad]回転
	}
	
	
	dBodySetRotation(body, R);
}

void RigidBody::SetRotation(const Vec3& angle)
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

	odeR[0]	= directR.r[0].m128_f32[0];
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

	dBodySetRotation(body, odeR);
}

DirectX::XMMATRIX RigidBody::GetRotation() const
{
	//DirextXMatrixとODEはRowMajor
	//ODEは1次元[4*3]で格納されているが、DirectXMathは2次元[4][4]

				/*ODE*/
	// | m[0] m[1] m[2]   m[3]  |
	// | m[4] m[5] m[6]   m[7]  |
	// | m[8] m[9] m[10] m[11]  |

			/*DirectXMath*/
	// float _11, _12, _13, _14;
	// float _21, _22, _23, _24;
	// float _31, _32, _33, _34;
	// float _41, _42, _43, _44;

	const dReal* odeR;
	odeR = dBodyGetRotation(body);
	
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

void RigidBody::BodyEnable()
{
	dBodyEnable(body);
}

void RigidBody::BodyDisable()
{
	dBodyDisable(body);
}

void DynamicBox::Create(const Vec3& pos, const Vec3& scale, dReal totalMass)
{
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, scale.x, scale.y, scale.z);
	dBodySetMass(body, &mass);
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetBody(geom, body);
	//ポジション
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

DynamicBox::DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass)
{
	Create(pos, scale, totalMass);
}

DynamicBox::DynamicBox(const DynamicBox& box)
{
	body = box.body;
	geom = box.geom;
}

DynamicBox::DynamicBox()
{
	dBodySetPosition(body, 0, 0, 0);
}

DynamicBox::~DynamicBox()
{
	
}


void DynamicSphere::Create(const Vec3& pos, const dReal& radius, const dReal totalMass)
{
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetSphereTotal(&mass, totalMass, radius);
	dBodySetMass(body, &mass);
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateSphere(Engine::GetPhysics().GetCollsionSpace(), radius);
	dGeomSetBody(geom, body);
	//ポジション
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

DynamicSphere::DynamicSphere(const Vec3& pos, const dReal& radius,dReal totalMass)
{
	Create(pos, radius, totalMass);
}

DynamicSphere::DynamicSphere(const DynamicSphere& sphere)
{
	body = sphere.body;
	geom = sphere.geom;
}

DynamicSphere::DynamicSphere()
{
	dBodySetPosition(body, 0, 0, 0);
}

DynamicSphere::~DynamicSphere()
{

}

DynamicCylinder::DynamicCylinder(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	Create(pos,  totalMass, direction, radius, length);
}

DynamicCylinder::DynamicCylinder(const DynamicCylinder& cylinder)
{
	body = cylinder.body;
	geom = cylinder.geom;
}

DynamicCylinder::DynamicCylinder()
{
	dBodySetPosition(body, 0, 0, 0);
}

DynamicCylinder::~DynamicCylinder()
{

}

void DynamicCylinder::Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetCylinderTotal(&mass, totalMass, direction, radius, length);
	dBodySetMass(body, &mass);
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateCylinder(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetBody(geom, body);

	//ポジション
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

DynamicCapsule::DynamicCapsule(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	Create(pos, totalMass, direction, radius, length);
}

DynamicCapsule::DynamicCapsule(const DynamicCapsule& capsule)
{
	body = capsule.body;
	geom = capsule.geom;
}

DynamicCapsule::DynamicCapsule()
{
	dBodySetPosition(body, 0, 0, 0);
}

DynamicCapsule::~DynamicCapsule()
{

}

void DynamicCapsule::Create(const Vec3& pos, const dReal totalMass, const int direction, const dReal radius, const dReal length)
{
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetCapsuleTotal(&mass, totalMass, direction, radius, length);
	dBodySetMass(body, &mass);
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateCapsule(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetBody(geom, body);
	//ポジション
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

