#include "Engine.h"

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	pGraphics = nullptr;
	vBuf = nullptr;
	textureShader = nullptr;
	texture = nullptr;
}

Engine::~Engine()
{
	if (pGraphics)
	{
		delete pGraphics;
		pGraphics = nullptr;
	}
	delete vBuf;
	delete textureShader;
	delete texture;
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

	textureShader = new TextureShader(pGraphics->GetDevice(),hwnd,"texture","VSMain","PSMain");

	texture = new Texture();
	texture->Create(pGraphics->GetDevice(), "mario-shell-sprite.png");

	vBuf = new VertexBuffer();
	vBuf->Create(pGraphics->GetDevice(), textureShader, 32.0f, false);
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
	Matrix view ,proj,world;
	

	D3DXVECTOR3 pos = D3DXVECTOR3(0,0,-100.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0,1.0f,0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0,0,1);

	view.MatrixLookAtLH(&view, pos.x, pos.y, pos.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
	proj.MatrixIdentity(&proj);
	world.MatrixIdentity(&world);

	textureShader->SetShaderParameters(pGraphics->GetDeviceContext(), texture->GetTexture());
	textureShader->SetShaderParameters(pGraphics->GetDeviceContext(), world, view, proj);

	vBuf->Draw(pGraphics->GetDeviceContext());

	//
	pGraphics->EndScene();
}

