#include "Engine.h"
#include <memory>

std::string Engine::title = "";

Engine::Engine(std::string WindowTitle)
{
	title = WindowTitle;
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

System& Engine::GetWindow()
{
	static std::unique_ptr<System>window(new System(title));
	return *window.get();
}

Direct3D & Engine::GetDirect3D()
{
	static std::unique_ptr<Direct3D> direct3D(new Direct3D());
	return *direct3D.get();
}
