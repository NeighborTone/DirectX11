#include "RigidBody.h"
#include "Engine.h"
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
	auto values = dBodyGetPosition(body);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void RigidBody::SetPosition(const Vec3& pos)
{
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void RigidBody::AddForce(const Vec3& force)
{
	//Memo::����𗘗p���邱�ƂŁA���̂��Ƃɏd�͂̒l��ݒ�ł���
	dBodyAddForce(body, force.x, force.y, force.z);
}

void RigidBody::AddVelocity(const Vec3 & velocity)
{
	dBodySetLinearVel(body, velocity.x, velocity.y, velocity.z);
}

Vec3 RigidBody::GetVelocity() const
{
	auto vel = dBodyGetLinearVel(body);
	return Vec3((float)vel[0], (float)vel[1], (float)vel[2]);
}

void RigidBody::SetQuaternion(Vec3& angle)
{
	//�܂��Ȃ̂œK��
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
	//�܂��Ȃ̂œK��
	auto val = dBodyGetQuaternion(body);

	return Vec3(
		DirectX::XMConvertToDegrees((float)val[1]), 
		DirectX::XMConvertToDegrees((float)val[2]), 
		DirectX::XMConvertToDegrees((float)val[3])
	);
}

void RigidBody::SetAxisAndAngle(Axis axis, float degree)
{
	//��̊p�x�������߂��Ȃ��̂ŗp�r�s�������ꉞ�p�ӂ��Ă�����
	dMatrix3 R;
	if (axis == Axis::X)
	{
		dRFromAxisAndAngle(R, 1.0, 0.0, 0.0, DirectX::XMConvertToRadians(degree));	//x�������[rad]��]
	}
	if (axis == Axis::Y)
	{
		dRFromAxisAndAngle(R, 0.0, 1.0, 0.0, DirectX::XMConvertToRadians(degree));	//y�������[rad]��]
	}
	if (axis == Axis::Z)
	{
		dRFromAxisAndAngle(R, 0.0, 0.0, 1.0, DirectX::XMConvertToRadians(degree));	//z�������[rad]��]
	}
	
	
	dBodySetRotation(body, R);
}

void RigidBody::SetRotation(const Vec3& angle)
{
	//�s��ɕϊ����Ďp�������߂�
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
	//DirextXMatrix��ODE��RowMajor
	//ODE��1����[4*3]�Ŋi�[����Ă��邪�ADirectXMath��2����[4][4]

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

	/*4*4�ɕϊ�����*/
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
	//�{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass, totalMass, scale.x, scale.y, scale.z);
	dBodySetMass(body, &mass);
	mesh.scale = scale;
	//�W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetBody(geom, body);
	mesh.pos = pos;
	//�|�W�V����
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void DynamicBox::Draw(Texture& tex)
{
	bool isInit = false;
	if (!isInit)
	{
		mesh.CreateCube();
		mesh.GetMaterial().SetTexture(0, &tex);
		mesh.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
		isInit = true;
	}
	mesh.pos = GetPosition();
	mesh.Draw(GetRotation());
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
	//�{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetSphereTotal(&mass, totalMass, radius);
	dBodySetMass(body, &mass);
	//�W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateSphere(Engine::GetPhysics().GetCollsionSpace(), radius);
	dGeomSetBody(geom, body);
	//�|�W�V����
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void DynamicSphere::Draw(Texture& tex)
{
	//���̂Ƃ���Ȃ�
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
	//�{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetCylinderTotal(&mass, totalMass, direction, radius, length);
	dBodySetMass(body, &mass);
	//�W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateCylinder(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetBody(geom, body);

	//�|�W�V����
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void DynamicCylinder::Draw(Texture& tex)
{
	//�܂��Ȃ�
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
	//�{�f�B������Ď��ʂ�ݒ肷��
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetCapsuleTotal(&mass, totalMass, direction, radius, length);
	dBodySetMass(body, &mass);
	//�W�I���g�����쐬���ă{�f�B���A�^�b�`
	geom = dCreateCapsule(Engine::GetPhysics().GetCollsionSpace(), radius, length);
	dGeomSetBody(geom, body);
	//�|�W�V����
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void DynamicCapsule::Draw(Texture& tex)
{
	//���Ȃ�
}

