#include "Engine.h"


Engine::Engine()
{
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
	return GetWindow().Run();
}

System& Engine::GetWindow()
{
	static std::unique_ptr<System>window(new System(Defs::WINDOW_TITLE));
	return *window.get();
}
