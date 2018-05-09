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

void Graphic::Run()
{
	Update();
	Draw();
}

void Graphic::Update()
{

}

void Graphic::Draw()
{
	directx.BeginScene(0, 0, 0, 1);
	directx.EndScene();
}
