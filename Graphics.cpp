#include "Graphics.h"


Graphics::Graphics()
{
	pManager = nullptr;
}


Graphics::~Graphics()
{
	if (pManager)
	{
		delete pManager;
		pManager = nullptr;
	}
}

bool Graphics::CreateDX(HWND hwnd)
{
	pManager = new DXManager;
	if (!pManager->Create(Defs::SCREEN_WIDTH, Defs::SCREEN_HEIGHT, Defs::VSYNC_ENABLED, hwnd, Defs::FULL_SCREEN))
	{
		return false;
	}
	return true;
}
void Graphics::Create()
{

}
void Graphics::BeginScene(float r, float g, float b, float a)
{
	pManager->BeginScene(r, g, b, a);
}
void Graphics::EndScene()
{
	pManager->EndScene();
}

void Graphics::EnableAlphaBlending(bool enable)
{
	pManager->EnableAlphaBlending(enable);
}

void Graphics::EnableZBuffer(bool enable)
{
	pManager->EnableZBuffer(enable);
}

DXManager* Graphics::GetDXManager()
{
	return pManager;
}

HWND Graphics::GetHwnd()
{
	return hwnd;
}

ID3D11Device* Graphics::GetDevice()
{
	return pManager->GetDevice();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return pManager->GetDeviceContext();
}

void Graphics::SetHwnd(HWND& hwnd)
{
	this->hwnd = hwnd;
}