#include "Engine.h"
#include <memory>

std::string Engine::title = "";
int Engine::width = 0;
int Engine::height = 0;
bool Engine::isFullScreen = false;
Engine::Engine(std::string WindowTitle, int width, int height,bool isFullScreen)
{
	title = WindowTitle;
	this->width = width;
	this->height = height;
	this->isFullScreen = isFullScreen;
	COMInitialize();
}

Engine::~Engine()
{

}

void Engine::COMInitialize()
{
	static bool isInitialized = false;

	if (!isInitialized)
	{
		//COM‚Ì‰Šú‰»
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		// ƒƒ‚ƒŠƒŠ[ƒNŒŸo
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		isInitialized = true;
	}
}

bool Engine::Run()
{
	GetDirect3D().Run();
	return GetWindow().Run();
}

HWND Engine::GetWindowHandle()
{
	return GetWindow().GetHwnd();
}

int Engine::GetWindowWidth()
{
	return width;
}

int Engine::GetWindowHeight()
{
	return height;
}

System& Engine::GetWindow()
{
	static std::unique_ptr<System>window(new System(title,width,height));
	return *window.get();
}

Direct3D & Engine::GetDirect3D()
{
	static std::unique_ptr<Direct3D> direct3D(new Direct3D(isFullScreen));
	return *direct3D.get();
}
