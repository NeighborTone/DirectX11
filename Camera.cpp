#include "Camera.h"
using namespace DirectX;
Camera::Camera():
	renderTargetView(nullptr),
	depthStencilView(nullptr),
	renderTexture(nullptr),
	depthTexture(nullptr),
	constantBuffer(nullptr),
	pos(0,0,0),
	angles(0, 0, 0),
	color(1, 1, 1, 1)
{
	Engine::COMInitialize();


	Create();
}


Camera::~Camera()
{

}

void Camera::SetPerspective(float fieldOfView, float nearClip, float farClip)
{
	isPerspective = true;
	this->fieldOfView = fieldOfView;
	this->nearClip = nearClip;
	this->farClip = farClip;
	float aspectRatio = Engine::GetWindowWidth() / (float)Engine::GetWindowHeight();
	constant.projection = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearClip, farClip));
}

void Camera::SetOrthographic(float size, float nearClip, float farClip)
{
	isPerspective = false;
	this->size = size;
	this->nearClip = nearClip;
	this->farClip = farClip;
	float aspectRatio = Engine::GetWindowWidth() / (float)Engine::GetWindowHeight();
	constant.projection = XMMatrixTranspose(
		XMMatrixOrthographicLH(size * aspectRatio, size, nearClip, farClip));
}

void Camera::SetDepthTest(bool isDepthTest)
{
	this->isDepthTest = isDepthTest;
}

void Camera::Update()
{
}

void Camera::Create()
{
}
