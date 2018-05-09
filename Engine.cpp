#include "Engine.h"
#include "SystemHelp.h"
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
using namespace DirectX;
Engine* Engine::instance = nullptr;
Engine::Engine()
{
	shader = nullptr;
	pGraphic = nullptr;
}

Engine::~Engine()
{

	
}

void Engine::Update()
{
	
}

void Engine::Draw()
{
	pGraphic->Begine();
	//

	/*仮のカメラ*/
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//ワールドトランスフォーム
	D3DXMatrixIdentity(&World);
	// ビュートランスフォーム
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //視点位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	// プロジェクショントランスフォーム
	D3DXMatrixPerspectiveFovLH(&Proj, (float(D3DX_PI / 4)), (FLOAT)SystemHelp::SCREEN_WIDTH / (FLOAT)SystemHelp::SCREEN_HEIGHT, 0.1f, 100.0f);
	
	//使用するシェーダーのセット
	
	//シェーダーのコンスタントバッファーに各種データを渡す
	

	//ワールド、カメラ、射影行列を渡す
		
	
	//このコンスタントバッファーをどのシェーダーで使うか
	

	//バーテックスバッファーをセット

	//頂点インプットレイアウトをセット
	//プリミティブ・トポロジーをセット
	//プリミティブをレンダリング

	//
	pGraphic->End();
}

void Engine::Run()
{
	Draw();
}

bool Engine::Create(HWND hwnd)
{
	pGraphic = std::make_unique<Graphic>(hwnd);
	shader = std::make_unique<Shader>();
	//シェーダー読み込み
	shader->SetLayOut();
	return true;
}

void Engine::Release()
{
	if (instance)
	{
		SystemHelp::SafeDelete(instance);
	}
	
}

Engine* Engine::GetEngine()
{
	if (instance == nullptr)
	{
		instance = new Engine();
	}
	return instance;
}

Graphic* Engine::GetGraphic()
{
	return pGraphic.get();
}
