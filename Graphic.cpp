#include "Graphic.h"
#include "SystemHelp.h"
Graphic::Graphic(HWND hwnd)
{
	this->hwnd = hwnd;
	directx.Create(SystemHelp::SCREEN_WIDTH, SystemHelp::SCREEN_HEIGHT, SystemHelp::VSYNC_ENABLED, this->hwnd, SystemHelp::FULL_SCREEN);
}

Graphic::~Graphic()
{

}

DirectX11* Graphic::GetDX()
{
	return &directx;
}

void Graphic::Run()
{
	Update();

}

void Graphic::Update()
{

}

void Graphic::Begine()
{
	directx.EnableAlphaBlending(true);
	directx.EnableZBuffer(true);
	directx.BeginScene(0, 0, 0, 1);
}

void Graphic::End()
{
	directx.EndScene();
}
