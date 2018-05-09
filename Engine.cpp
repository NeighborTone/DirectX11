#include "Engine.h"
#include "SystemDefs.h"

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	pGraphics = nullptr;

}

Engine::~Engine()
{
	Memory::SafeDelete(pGraphics);
	Memory::SafeDelete(pSprite);
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
	TextureShader* shader = new TextureShader(pGraphics->GetDevice(), hwnd,"texture", "VSMain", "PSMain");
	//pSprite = new Sprite(100.f);	//”{—¦H
	pSprite = new AnimatedSprite(64.0f, 24.0f);
	pSprite->Create(pGraphics->GetDevice(),pGraphics->GetDeviceContext(), shader, "Texture/mario-shell-sprite.png");
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
	pSprite->Update();
}

void Engine::Draw()
{
	pGraphics->EnableAlphaBlending(true);
	pGraphics->BeginScene(0, 0, 0, 1);

	//‚±‚±‚É•`‰æˆ—‚ð•`‚­
	//‰¼‚ÌƒJƒƒ‰
	D3DXMATRIX view ,proj,world;
	D3DXVECTOR3 pos = D3DXVECTOR3(0,0,-100.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0,1.0f,0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0,0,1);

	D3DXMatrixLookAtLH(&view, &pos, &lookAt, &up);
	D3DXMatrixOrthoLH(&proj, (float)Defs::SCREEN_WIDTH, (float)Defs::SCREEN_HEIGHT, 0.1f, 1000.0f);
	D3DXMatrixIdentity(&world);

	pSprite->Render(pGraphics->GetDeviceContext(), world, view, proj);
	
	//
	pGraphics->EndScene();
}

