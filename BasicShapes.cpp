#include "BasicShapes.h"
#include "Engine.h"

using namespace DirectX;


BallShape::BallShape():
	ball(nullptr),
	pos(0,0,0),
	angle(0,0,0),
	scale(1,1,1)
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
		false,			//����:false�ł₪�o��?
		false);			//����:false�ł₪�o��?

	// ���[���h�}�g���b�N�X�̏�����
	constant.world = XMMatrixIdentity();

	// �r���[�}�g���b�N�X�̏�����
	XMVECTOR hEye = XMVectorSet(camera.pos.x, camera.pos.y, camera.pos.z, 0.0f);				//�J�����̈ʒu
	XMVECTOR hAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);		//�œ_�̈ʒu
	XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	constant.view = XMMatrixLookAtLH(hEye, hAt, hUp);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	constant.projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, Engine::GetWindowSize().x / (FLOAT)Engine::GetWindowSize().y, 0.001f, 10000.0f);
}

void BallShape::Draw(Camera& camera)
{
	static float t = 0;
	t++;
	//�������_�𒆐S�ɉ�]
	constant.world = XMMatrixRotationY(t);
	XMMATRIX local = XMMatrixMultiply(constant.world, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	ball->Draw(local, constant.view, constant.projection, Colors::CornflowerBlue,nullptr,true);

}
