#include "Camera.h"

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
}

void Camera::SetOrthographic(float size, float nearClip, float farClip)
{
}

void Camera::SetDepthTest(bool isDepthTest)
{
}

void Camera::Update()
{
}

void Camera::Create()
{
}
