#include "RigidBody.h"
#include "Engine.h"

RigidBody::RigidBody() :
	body(nullptr),
	geom(nullptr),
	pos(0, 0, 0)
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
Vec3 RigidBody::GetPosition() const
{
	auto values = dBodyGetPosition(body);
	return Vec3((float)values[0], (float)values[1], (float)values[2]);
}

void RigidBody::SetPosition(const Vec3& pos)
{
	this->pos = pos;
	dBodySetPosition(body, this->pos.x, this->pos.y, this->pos.z);
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

Vec3 RigidBody::GetVelocity()
{
	auto vel = dBodyGetLinearVel(body);
	return Vec3((float)vel[0], (float)vel[1], (float)vel[2]);
}

void RigidBody::SetAngle(Vec3& angle)
{
	dQuaternion q = { 1,angle.x,angle.y,angle.z };

	dBodySetQuaternion(body, q);

}

Vec3 RigidBody::GetAngle()
{
	auto val = dBodyGetQuaternion(body);

	return Vec3(DirectX::XMConvertToDegrees((float)val[1]), DirectX::XMConvertToDegrees((float)val[2]), DirectX::XMConvertToDegrees((float)val[3]));
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
	mesh.scale = scale;
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateBox(Engine::GetPhysics().GetCollsionSpace(), scale.x, scale.y, scale.z);
	dGeomSetBody(geom, body);
	mesh.pos = pos;
	//ポジション
	dBodySetPosition(body, mesh.pos.x, mesh.pos.y, mesh.pos.z);
}

void DynamicBox::Draw(Texture& tex)
{
	static bool isInit = false;
	if (!isInit)
	{
		mesh.CreateCube();
		mesh.GetMaterial().SetTexture(0, &tex);
		mesh.SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
	}
	mesh.pos = GetPosition();
	mesh.angle = GetAngle();
	mesh.Draw();
}

DynamicBox::DynamicBox(const Vec3& pos, const Vec3& scale, dReal totalMass)
{
	this->pos = pos;
	Create(this->pos, scale, totalMass);
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


void DynamicSphere::Create(const Vec3 & pos, const dReal & r, dReal totalMass)
{
	//ボディを作って質量を設定する
	body = dBodyCreate(Engine::GetPhysics().GetWorld());
	dMass mass;
	dMassSetZero(&mass);
	dMassSetSphereTotal(&mass, totalMass, r);
	dBodySetMass(body, &mass);
	//ジオメトリを作成してボディをアタッチ
	geom = dCreateSphere(Engine::GetPhysics().GetCollsionSpace(), r);
	dGeomSetBody(geom, body);
	this->pos = pos;
	//ポジション
	dBodySetPosition(body, this->pos.x, this->pos.y, this->pos.z);
}

void DynamicSphere::Draw(Texture & tex)
{
	//なし
}

DynamicSphere::DynamicSphere(const Vec3& pos, const dReal& r,dReal totalMass)
{
	this->pos = pos;
	Create(this->pos, r, totalMass);
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

