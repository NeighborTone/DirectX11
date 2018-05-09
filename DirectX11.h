#pragma once
#pragma warning( disable : 4005 )	
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dCompiler.lib")
class DirectX11
{
public:
	DirectX11();
	~DirectX11();

	//DirectX11�̏�����
	bool Create(int width, int height, bool vsync, HWND hwnd, bool isfullscreen);
	//�`��J�n
	void BeginScene(float r, float g, float b, float a);							
	//��ʍX�V
	void EndScene();
	//�A���t�@�u�����h��؂�ւ���
	void EnableAlphaBlending(bool enable);
	//Z�o�b�t�@�̐؂�ւ�
	void EnableZBuffer(bool enable);
	//�f�o�C�X�𓾂�
	ID3D11Device* GetDevice();
	//�R���e�L�X�g�𓾂�
	ID3D11DeviceContext* GetDeviceContext();
private:
	//�X���b�v�`�F�C���ƃf�o�C�X�̏�����
	bool InitSwapChain(HWND hwnd, bool isfullscreen, int width, int height, unsigned  int numerator, unsigned int denominator);
	//�[�x�o�b�t�@
	bool InitDepthBuffer(int width, int height);
	//�X�e���V���o�b�t�@
	bool InitDepthStencilBuffer();
	//�X�e���V���r���[
	bool InitStencilView();
	//���X�^���C�U�[
	bool InitRasterizerState();
	//�r���[�|�[�g
	void InitViewport(int width, int height);
	//�A���t�@�u�����f�B���O
	bool InitAlphaBlending();
	//Z�o�b�t�@
	bool InitZBuffer();

	bool vsyncEnabled;
	int videoCardMemory;
	char videoCardDescription[128];

	IDXGISwapChain*				pSwapChain;
	ID3D11Device*					pDevice;
	ID3D11DeviceContext*		pDeviceContext;
	ID3D11RenderTargetView*	pRenderTargetView;
	ID3D11Texture2D*				pDepthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilView*	pDepthStencilView;
	ID3D11RasterizerState*	pRasterState;
	ID3D11BlendState*			pAlphaEnableBlendingState;
	ID3D11BlendState*			pAlphaDisableBlendingState;
	ID3D11DepthStencilState* pDepthDisabledStencilState;
};

