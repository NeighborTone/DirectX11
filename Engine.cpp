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

	/*���̃J����*/
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//���[���h�g�����X�t�H�[��
	D3DXMatrixIdentity(&World);
	// �r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //���_�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[��
	D3DXMatrixPerspectiveFovLH(&Proj, (float(D3DX_PI / 4)), (FLOAT)SystemHelp::SCREEN_WIDTH / (FLOAT)SystemHelp::SCREEN_HEIGHT, 0.1f, 100.0f);
	
	//�g�p����V�F�[�_�[�̃Z�b�g
	
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	

	//���[���h�A�J�����A�ˉe�s���n��
		
	
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	//�v���~�e�B�u�������_�����O

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
	//�V�F�[�_�[�ǂݍ���
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
