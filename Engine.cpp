#include "Engine.h"

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	pGraphics = nullptr;
}

Engine::~Engine()
{
	if (pGraphics)
	{
		delete pGraphics;
		pGraphics = nullptr;
	}
}


//public----------------------------------------------
bool Engine::CreateGraphics(HWND hwnd)
{
	pGraphics = new Graphics();

	return pGraphics->CreateDX(hwnd);
}

bool Engine::Create(HINSTANCE hinstance, HWND hwnd)
{
	pGraphics->Create();
	return true;
}

void Engine::Run()
{
	Update();
	Draw();
}

void Engine::Release()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

Graphics* Engine::GetGraphics()
{
	return pGraphics;
}

Engine* Engine::GetInst()
{
	if (instance == nullptr)
	{
		instance = new Engine();
	}
	return instance;
}

//private-------------------------------------------------------------
void Engine::Update()
{

}

void Engine::Draw()
{
	pGraphics->BeginScene(0, 0, 0, 1);

	//‚±‚±‚É•`‰æˆ—‚ð•`‚­

	pGraphics->EndScene();
}

