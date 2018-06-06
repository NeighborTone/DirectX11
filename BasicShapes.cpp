#include "BasicShapes.h"
#include "Engine.h"

using namespace DirectX;


BallShape::BallShape():
	ball(nullptr),
	pos(0,0,0),
	angle(0,0,0)
{
	
}


BallShape::~BallShape()
{
}

void BallShape::Create(float r, size_t tessellation, Camera& camera)
{
	
	this->r = r;
	ball = GeometricPrimitive::CreateSphere
	(
		&Engine::GetDXContext3D(),
		this->r * 2,			//���a
		tessellation,	//���_��
		false,			//false�ō���
		false);			//����:false�ł₪�o��?

	//���[���h�̏�����
	constant.world = XMMatrixIdentity();
	//�r���[�}�g���b�N�X�̏�����
	XMVECTOR hEye = XMVectorSet(camera.pos.x, camera.pos.y, camera.pos.z, 0.0f);				//�J�����̈ʒu
	//�œ_�̈ʒu
	XMVECTOR hAt = XMVectorSet(
		XMConvertToRadians(camera.angles.x),
		XMConvertToRadians(camera.angles.y),
		XMConvertToRadians(camera.angles.z), 
		0.0f);		
	XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	constant.view = XMMatrixLookAtLH(hEye, hAt, hUp);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	constant.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), Engine::GetWindowSize().x / (FLOAT)Engine::GetWindowSize().y, 1, 10000.0f);
}

void BallShape::Draw(Camera& camera)
{
	pos.x = 2;
	static float t = 0;
	t++;
	//�������_�𒆐S�ɉ�]
	constant.world = XMMatrixRotationX(XMConvertToRadians(angle.x));
	constant.world = XMMatrixRotationY(XMConvertToRadians(angle.y));
	constant.world = XMMatrixRotationZ(XMConvertToRadians(angle.z = t));
	XMMATRIX local = XMMatrixMultiply(constant.world, XMMatrixTranslation(pos.x, pos.y, pos.z));
	ball->Draw(local, constant.view, constant.projection, Colors::CornflowerBlue,nullptr,true);

}
