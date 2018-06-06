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
		this->r * 2,			//直径
		tessellation,	//頂点数
		false,			//falseで左手
		false);			//質感:falseでつやが出る?

	//ワールドの初期化
	constant.world = XMMatrixIdentity();
	//ビューマトリックスの初期化
	XMVECTOR hEye = XMVectorSet(camera.pos.x, camera.pos.y, camera.pos.z, 0.0f);				//カメラの位置
	//焦点の位置
	XMVECTOR hAt = XMVectorSet(
		XMConvertToRadians(camera.angles.x),
		XMConvertToRadians(camera.angles.y),
		XMConvertToRadians(camera.angles.z), 
		0.0f);		
	XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	constant.view = XMMatrixLookAtLH(hEye, hAt, hUp);

	// プロジェクションマトリックスの初期化
	constant.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), Engine::GetWindowSize().x / (FLOAT)Engine::GetWindowSize().y, 1, 10000.0f);
}

void BallShape::Draw(Camera& camera)
{
	pos.x = 2;
	static float t = 0;
	t++;
	//球を原点を中心に回転
	constant.world = XMMatrixRotationX(XMConvertToRadians(angle.x));
	constant.world = XMMatrixRotationY(XMConvertToRadians(angle.y));
	constant.world = XMMatrixRotationZ(XMConvertToRadians(angle.z = t));
	XMMATRIX local = XMMatrixMultiply(constant.world, XMMatrixTranslation(pos.x, pos.y, pos.z));
	ball->Draw(local, constant.view, constant.projection, Colors::CornflowerBlue,nullptr,true);

}
