#include "DirectX11.h"
#include "SystemHelp.h"

DirectX11::DirectX11() :
	pSwapChain(nullptr),
	pDevice(nullptr),
	pDeviceContext(nullptr),
	pRenderTargetView(nullptr),
	pDepthStencilBuffer(nullptr),
	pDepthStencilState(nullptr),
	pDepthStencilView(nullptr),
	pRasterState(nullptr),
	pAlphaEnableBlendingState(nullptr),
	pAlphaDisableBlendingState(nullptr),
	pDepthDisabledStencilState(nullptr)
{

}
DirectX11::~DirectX11()
{
	SystemHelp::SafeRelease(pDevice);
	SystemHelp::SafeRelease(pDeviceContext);
	SystemHelp::SafeRelease(pRenderTargetView);
	SystemHelp::SafeRelease(pDepthStencilBuffer);
	SystemHelp::SafeRelease(pDepthStencilState);
	SystemHelp::SafeRelease(pDepthStencilView);
	SystemHelp::SafeRelease(pRasterState);
	SystemHelp::SafeRelease(pAlphaEnableBlendingState);
	SystemHelp::SafeRelease(pAlphaDisableBlendingState);
	SystemHelp::SafeRelease(pDepthDisabledStencilState);

}
//public-------------------------------------------------------------------------------------------------------------------------
bool  DirectX11::Create(int width, int height, bool vsync, HWND hwnd, bool isfullscreen)
{
	HRESULT hr;
	//DirectX�O���t�B�b�N�X�C���t���X�g���N�`���[(DXGI)
	IDXGIFactory* factory;			//�t���X�N���[���؂�ւ�����������
	IDXGIAdapter* adapter;			//�f�B�X�v���C�\�����s�Ȃ����߂̃f�o�C�X(�f�B�X�v���C�T�u�V�X�e��)
	IDXGIOutput* adapterOutput;		//���j�^�[�Ȃǂɏo�͂��邽�߂̃C���^�[�t�F�[�X

	unsigned int
		numModes,
		numerator = 0,
		denominator = 0,
		stringLength;

	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	ID3D11Texture2D* backBufferPtr;

	//���������ݒ�
	vsyncEnabled = vsync;

	//DirectXGraphic�C���^�[�t�F�C�X�̍쐬
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectXGraphic�C���^�[�t�F�C�X�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//DirectXGraphic�C���^�[�t�F�C�X�t�@�N�g���[�̍쐬
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectXGraphic�C���^�[�t�F�C�X�t�@�N�g���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�v���C�}���A�_�v�^�̏o�͂�񋓂���
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�v���C�}���A�_�v�^�̏o�͂Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�v�����ꂽ�f�B�X�v���C�̃t�H�[�}�b�g�ɓK������t�H�[�}�b�g�̐����擾
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,		//�J���[�t�H�[�}�b�g
		DXGI_ENUM_MODES_INTERLACED,		//�I�v�V����
		&numModes,								//���[�h�����Ԃ�
		NULL);									//�\�������[�h�X�g�ւ̃|�C���^�[NULL �ɐݒ肷��ƁA�\�����[�h�̐����擾�ł���
	if (FAILED(hr))
	{
		MessageBox(NULL, "�f�B�X�v���C�t�H�[�}�b�g�̎擾�Ɏ��s", "Error", MB_OK);
		return false;
	}
	//�g�p���Ă��郂�j�^�[�ƃr�f�I�J�[�h�Ŏg�p�\�ȃ��[�h��ێ����郊�X�g���쐬����
	displayModeList = new DXGI_MODE_DESC[numModes];

	//�擾�����\�����[�h���X�g�Ń��[�h���w�肷��
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�f�B�X�v���C�t�H�[�}�b�g�̎w��Ɏ��s", "Error", MB_OK);
		return false;
	}

	for (int i = 0; i < (int)numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)width)
		{
			if (displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//�������������s��������Ƃ����ň���������
	if (numerator == 0 && denominator == 0)
	{
		//MessageBox(NULL, "���������Ɏ��s", "warning", S_OK);
		numerator = 60;
		denominator = 1;
	}

	//�A�_�v�^�[(�܂��̓r�f�I�J�[�h)��DXGI1.0�L�q���擾
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�A�_�v�^�[�̎擾�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�r�f�I�J�[�h�̃����������K�o�C�g�P�ʂŊi�[����
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�r�f�I�J�[�h�̖��O�𕶎��z��ɕϊ�����
	error = wcstombs_s(
		&stringLength,					//�ϊ����ꂽ�������B
		videoCardDescription,		//�ϊ����ꂽ�}���`�o�C�g�����̕�����𐶐������o�b�t�@�[�̃A�h���X�B
		128,								//�T�C�Y�̃o�C�g��
		adapterDesc.Description,	//�ϊ����郏�C�h�����̕�����ւ̃|�C���^�[�B
		128);								//�i�[�����o�C�g�̍ő吔�A
	if (error != 0)
	{
		MessageBox(NULL, "�r�f�I�J�[�h���擾�ł��܂���ł���", "Error", MB_OK);
		return false;
	}

	//��������Ȃ��̂Ŕj��
	SystemHelp::SafeDeleteArr(displayModeList);
	SystemHelp::SafeRelease(adapterOutput);
	SystemHelp::SafeRelease(adapter);
	SystemHelp::SafeRelease(factory);

	if (!InitSwapChain(hwnd, isfullscreen, width, height, numerator, denominator))
	{
		return false;
	}

	//�o�b�N�o�b�t�@�[�ւ̃|�C���^�[�𓾂�
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�o�b�N�o�b�t�@�[�̎Q�ƂɎ��s", "Error", MB_OK);
		return false;
	}

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	hr = pDevice->CreateRenderTargetView(backBufferPtr, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	//�o�b�N�o�b�t�@�̔j��
	SystemHelp::SafeRelease(backBufferPtr);

	if (!InitDepthBuffer(width, height))
	{
		return false;
	}

	if (!InitDepthStencilBuffer())
	{
		return false;
	}

	if (!InitStencilView())
	{
		return false;
	}

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���o�b�t�@���o�̓����_�����O�p�C�v���C���Ƀo�C���h����
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	if (!InitRasterizerState())
	{
		return false;
	}

	InitViewport(width, height);

	if (!InitAlphaBlending())
	{
		return false;
	}

	if (!InitZBuffer())
	{
		return false;
	}

	return true;

}

void  DirectX11::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	//�w�i�N���A�F
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;


	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void DirectX11::EndScene()
{
	if (vsyncEnabled)
	{
		//60fps�ŕ`��
		pSwapChain->Present(1, 0);
		//30fps
		//pSwapChain->Present(2, 0);
	}
	else
	{
		//�����������Ƃ�Ȃ��ŕ`��
		pSwapChain->Present(0, 0);
	}
}

void  DirectX11::EnableAlphaBlending(bool enable)
{
	//�A���t�@�l���L�����������ŏ�����ς���
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;
	blendFactor[0] = 0.0f;

	if (enable)
	{
		pDeviceContext->OMSetBlendState(pAlphaEnableBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState, blendFactor, 0xffffffff);
	}
}
void  DirectX11::EnableZBuffer(bool enable)
{
	//Z�o�b�t�@�[���L�����������ŏ�����ς���
	if (enable)
	{
		pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	}
	else
	{
		pDeviceContext->OMSetDepthStencilState(pDepthDisabledStencilState, 1);
	}
}

ID3D11Device*  DirectX11::GetDevice()
{
	return pDevice;
}
ID3D11DeviceContext*  DirectX11::GetDeviceContext()
{
	return pDeviceContext;
}


//private------------------------------------------------------------------------------------------------------------------------
bool DirectX11::InitSwapChain(HWND hwnd, bool isfullscreen, int width, int height, unsigned  int numerator, unsigned int denominator)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr;

	SecureZeroMemory(&swapDesc, sizeof(swapDesc));

	//�V���O���o�b�N�o�b�t�@(�����)��ݒ�
	swapDesc.BufferCount = 1;

	//�o�b�N�o�b�t�@�̃E�B���h�E����ݒ�
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;

	//�o�b�N�o�b�t�@��32bit�ɐݒ�
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//���t���b�V�����[�g�̐ݒ�
	if (vsyncEnabled)
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Usage�̐ݒ�
	//Usage�̓e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��I�v�V����
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//�E�B���h�E�n���h���̐ݒ�
	swapDesc.OutputWindow = hwnd;

	//�}���`�T���v�����O���I�t�ɂ���
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//�E�B���h�E���[�h�ݒ�
	swapDesc.Windowed = !isfullscreen;

	//�������̏����ƃX�P�[�����O��s����ɐݒ肷��
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//�\���ς݂̃o�b�N�o�b�t�@�̔j��
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapDesc.Flags = 0;

	//DirectX11�̋@�\���x���̐ݒ�
	D3D_FEATURE_LEVEL feature[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//�X���b�v�`�F�C��(�t�����g�o�b�t�@)�ƃR���e�L�X�g�̍쐬
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,									//�f�o�C�X�̍쐬���Ɏg�p����r�f�I�A�_�v�^�[�ւ̃|�C���^�[
		D3D_DRIVER_TYPE_HARDWARE,	//�쐬����f�o�C�X�̎��
		NULL,									//�\�t�g�E�F�A���X�^���C�U�[����������DLL�̃n���h��
		0,										//�L���ɂ��郉���^�C�����C���[
		feature,								//�쐬�����݂�@�\���x���̏������w�肷��D3D_FEATURE_LEVEL�̔z��ւ̃|�C���^�[
		6,										//pFeatureLevels�̗v�f��
		D3D11_SDK_VERSION,				//SDK�̃o�[�W�����BD3D11_SDK_VERSION���w��
		&swapDesc,							//�X���b�v�`�F�[���̏������p�����[�^�[���i�[����X���b�v�`�F�[���̋L�q�ւ̃|�C���^�[
		&pSwapChain,						//�����_�����O�Ɏg�p����X���b�v �`�F�[����\��IDXGISwapChain�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
		&pDevice,							//�쐬���ꂽ�f�o�C�X��\��ID3D11Device�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ��BNULL���w�肷��ƁApFeatureLevel�ŃT�|�[�g����Ă���ō��̋@�\���x�����Ԃ����
		NULL,									//���̃f�o�C�X�ŃT�|�[�g����Ă���@�\���x���̔z��ɂ���ŏ��̗v�f��\��D3D_FEATURE_LEVEL�ւ̃|�C���^�[��Ԃ�
		&pDeviceContext);				//�f�o�C�X �R���e�L�X�g��\��ID3D11DeviceContext�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
	if (FAILED(hr))
	{
		MessageBox(NULL, "�X���b�v�`�F�[���̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	return true;
}
bool DirectX11::InitDepthBuffer(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthBuf;
	HRESULT hr;
	SecureZeroMemory(&depthBuf, sizeof(depthBuf));

	//�f�v�X�o�b�t�@(Z�o�b�t�@)�̐ݒ�
	depthBuf.Width = width;											//��
	depthBuf.Height = height;										//����
	depthBuf.MipLevels = 1;											//�~�b�v�}�b�v(�e�N�X�`���̉摜��⊮����)���x���̍ő吔
	depthBuf.ArraySize = 1;											//�e�N�X�`���[�z����̃e�N�X�`���[�̐�
	depthBuf.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//�e�L�X�`���[�t�H�[�}�b�g
	depthBuf.SampleDesc.Count = 1;								//�}���`�T���v�����O(�A���`�G�C���A�V���O)�p�����[�^�[���w�肷��\����
	depthBuf.SampleDesc.Quality = 0;							//�}���`�T���v�����O(�A���`�G�C���A�V���O)�p�����[�^�[���w�肷��\����
	depthBuf.Usage = D3D11_USAGE_DEFAULT;					//�e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
	depthBuf.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//�p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
	depthBuf.CPUAccessFlags = 0;									//������CPU�A�N�Z�X�̎�ނ��w�肷��t���O
	depthBuf.MiscFlags = 0;											//���̈�ʐ��̒Ⴂ���\�[�X �I�v�V���������ʂ���t���O

																	//�[�x�o�b�t�@�̍쐬
	hr = pDevice->CreateTexture2D(
		&depthBuf,
		NULL,
		&pDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�[�x�o�b�t�@�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	return true;
}
bool DirectX11::InitDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//�f�X�N���v�V�����̐ݒ�
	depthStencilDesc.DepthEnable = true;							//�[�x�e�X�g���g�p�\�ɂ���
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//�[�x�X�e���V���o�b�t�@�[�̒��ŁA�[�x�f�[�^�ɂ��ύX���\�ȕ���������
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;				//�[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r����֐�

	depthStencilDesc.StencilEnable = true;							//�X�e���V���e�X�g���g�p�\�ɂ��܂��B
	depthStencilDesc.StencilReadMask = 0xFF;						//�[�x�X�e���V���o�b�t�@�[�̒��ŁA�X�e���V���f�[�^��ǂݎ�镔��������
	depthStencilDesc.StencilWriteMask = 0xFF;						//�[�x�X�e���V���o�b�t�@�[�̒��ŁA�X�e���V���f�[�^���������ޕ���������

																	//�@�����J�����̕����������Ă���T�[�t�F�X(�\��)�����s�N�Z���̐[�x�e�X�g�ƃX�e���V���e�X�g�̌��ʂ��g�p������@������
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�@�����J�����Ƌt�����������Ă���T�[�t�F�X�����s�N�Z���̐[�x�e�X�g�ƃX�e���V�� �e�X�g�̌��ʂ��g�p������@������
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�[�x�X�e���V���X�e�[�g�̍쐬
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�[�x�X�e���V���X�e�[�g�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�R���e�L�X�g�ɐ[�x�X�e���V���X�e�[�g��ݒ�
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	return true;
}
bool DirectX11::InitStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//�[�x�X�e���V�� �r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X���w�肷��
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		//���\�[�X�̃f�[�^�t�H�[�}�b�g
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//���\�[�X�̃^�C�v
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//�f�v�X�X�e���V���r���[�̍쐬
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�[�x�X�e���V���r���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	return true;
}
bool DirectX11::InitRasterizerState()
{
	//���X�^���C�U�[�X�e�[�g�́A�ʒu�A�J���[�A�e�N�X�`���[�Ȃǂ�3D�f�[�^��2D�T�[�t�F�C�X��Ƀ����_�����O������@�����肷��	
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT hr;

	rasterDesc.AntialiasedLineEnable = false;	//���̃A���`�G�C���A�V���O��L���B����`�撆��MultisampleEnable��false�̏ꍇ�ɂ̂ݓK�p
	rasterDesc.CullMode = D3D11_CULL_BACK;		//�w��̕����������Ă���O�p�`���`�悳��Ȃ����Ƃ�����
	rasterDesc.DepthBias = 0;					//�w��̃s�N�Z���ɉ��Z����[�x�l
	rasterDesc.DepthBiasClamp = 0.0f;			//�s�N�Z���̍ő�[�x�o�C�A�X(�΂�)
	rasterDesc.DepthClipEnable = true;			//�����Ɋ�Â��ăN���b�s���O��L��
	rasterDesc.FillMode = D3D11_FILL_SOLID;		//�����_�����O���Ɏg�p����`�惂�[�h������
	rasterDesc.FrontCounterClockwise = false;	//�O�p�`���O��������������������B�̃p�����[�^�[��true�̏ꍇ�A�O�p�`�̒��_�������_�[�^�[�Q�b�g��ō����Ȃ�ΎO�p�`�͑O�����ƌ��Ȃ���A�E���Ȃ�Ό������ƌ��Ȃ���A���̃p�����[�^�[��false�̏ꍇ�͋t�ɂȂ�
	rasterDesc.MultisampleEnable = false;		//�}���`�T���v�����O�̃A���`�G�C���A�V���O��L��
	rasterDesc.ScissorEnable = false;			//�V�U�[��`�J�����O��L��
	rasterDesc.SlopeScaledDepthBias = 0.0f;		//�w��̃s�N�Z���̃X���[�v�ɑ΂���X�J��

	hr = pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���X�^���C�U�[�X�e�[�g�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	pDeviceContext->RSSetState(pRasterState);

	return true;
}
void DirectX11::InitViewport(int width, int height)
{
	D3D11_VIEWPORT viewport;

	//�r���[�|�[�g�̐ݒ�
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;


	pDeviceContext->RSSetViewports(1, &viewport);
}
bool DirectX11::InitAlphaBlending()
{
	D3D11_BLEND_DESC blendStateDesc;
	HRESULT hr;
	SecureZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	//�A���t�@�u�����f�B���O�̐ݒ�
	blendStateDesc.RenderTarget[0].BlendEnable = true;							//�u�����f�B���O��L��(�܂��͖���) 
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//�ŏ���RGB�f�[�^�\�[�X���w��
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//2�Ԗڂ�RGB�f�[�^�\�[�X���w��
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//RGB�f�[�^�\�[�X�̑g�������@���`
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//�ŏ��̃A���t�@ �f�[�^ �\�[�X���w��
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//2�Ԗڂ̃A���t�@�f�[�^�\�[�X���w��
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//�A���t�@ �f�[�^ �\�[�X�̑g�������@���`
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;				//�������݃}�X�N

																				//�L���ȃA���t�@�u�����h��Ԃ��쐬����
	hr = pDevice->CreateBlendState(&blendStateDesc, &pAlphaEnableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	//�����ɂ����A���t�@�u�����h�̏�Ԃ��쐬����悤�ɕύX����
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	//�����ȃA���t�@�u�����h�̍쐬
	hr = pDevice->CreateBlendState(&blendStateDesc, &pAlphaDisableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
bool DirectX11::InitZBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT hr;
	SecureZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));


	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthDisabledStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}