#include "Direct3D.h"
#include "Engine.h"
#include <array>
bool Direct3D::isFullScreen = false;
Direct3D::Direct3D(bool isFullScreen) :
	device3D(nullptr),
	context3D(nullptr),
	device2D(nullptr),
	context2D(nullptr),
	swapChain(nullptr),
	textureFactory(nullptr),
	textFactory(nullptr)
{
	this->isFullScreen =  isFullScreen;
	Engine::COMInitialize();
	Create(this->isFullScreen);
}


Direct3D::~Direct3D()
{
	//���ꂪ�Ȃ��ƃt���X�N���[���I�����ɐ���Ƀo�O��
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}
	System::RemoveProcedure(this);
}

ID3D11Device& Direct3D::GetDevice3D() const
{
	return *device3D;
}

ID3D11DeviceContext& Direct3D::GetContext3D() const
{
	return *context3D;
}

ID2D1Device& Direct3D::GetDevice2D() const
{
	return *device2D;
}

ID2D1DeviceContext& Direct3D::GetContext2D() const
{
	return *context2D;
}

IDXGISwapChain& Direct3D::GetSwapChain() const
{
	return *swapChain;
}

IWICImagingFactory& Direct3D::GetTextureFactory() const
{
	return *textureFactory;
}

IDWriteFactory& Direct3D::GetTextFactory() const
{
	return *textFactory.Get();
}

void Direct3D::Run()
{
	swapChain->Present(1, 0);
}

bool Direct3D::Create(bool isFull)
{

	if (!IsCreateSwapChain(isFull))
	{
		MessageBox(NULL, "�X���b�v�`�F�[���̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	context3D->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (!IsCreateBlendState())
	{
		MessageBox(NULL, "�A���t�@�u�����h�̐ݒ�Ɏ��s", "Error", MB_OK);
		return false;
	}
	if (!IsCreateD2D())
	{
		MessageBox(NULL, "Direct2D�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	System::AddProcedure(this);
	SetViewport();

	return true;
}

bool Direct3D::IsCreateSwapChain(bool isFull)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr;
	SecureZeroMemory(&swapDesc, sizeof(swapDesc));

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//Direct2D��DirectX11��A�g������̂ɕK�{
#if defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;		//�f�o�b�O�p
#endif

	//�V���O���o�b�N�o�b�t�@(�����)��ݒ�
	swapDesc.BufferCount = 1;

	//�o�b�N�o�b�t�@�̃E�B���h�E����ݒ�
	swapDesc.BufferDesc.Width = Engine::GetWindowWidth();
	swapDesc.BufferDesc.Height = Engine::GetWindowHeight();

	//�o�b�N�o�b�t�@��32bit�ɐݒ�
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//���t���b�V�����[�g�̐ݒ�
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	//Usage�̐ݒ�
	//Usage�̓e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��I�v�V����
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//�E�B���h�E�n���h���̐ݒ�
	swapDesc.OutputWindow = Engine::GetWindowHandle();

	//�}���`�T���v�����O���I�t�ɂ���
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//�E�B���h�E���[�h�ݒ�
	swapDesc.Windowed = isFull;

	//�������̏����ƃX�P�[�����O��s����ɐݒ肷��
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//�\���ς݂̃o�b�N�o�b�t�@�̔j��
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapDesc.Flags = 0;

		//�h���C�o�[�^�C�v�̗�
	std::array<D3D_DRIVER_TYPE, 5> driverType =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE,
	};
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
	for (size_t i = 0; i < driverType.size(); i++)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,									//�f�o�C�X�̍쐬���Ɏg�p����r�f�I�A�_�v�^�[�ւ̃|�C���^�[
			driverType[i],						//�쐬����f�o�C�X�̎��
			NULL,									//�\�t�g�E�F�A���X�^���C�U�[����������DLL�̃n���h��
			flags,								//�L���ɂ��郉���^�C�����C���[
			feature,								//�쐬�����݂�@�\���x���̏������w�肷��D3D_FEATURE_LEVEL�̔z��ւ̃|�C���^�[
			6,										//pFeatureLevels�̗v�f��
			D3D11_SDK_VERSION,				//SDK�̃o�[�W�����BD3D11_SDK_VERSION���w��
			&swapDesc,							//�X���b�v�`�F�[���̏������p�����[�^�[���i�[����X���b�v�`�F�[���̋L�q�ւ̃|�C���^�[
			&swapChain,							//�����_�����O�Ɏg�p����X���b�v �`�F�[����\��IDXGISwapChain�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
			&device3D,							//�쐬���ꂽ�f�o�C�X��\��ID3D11Device�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ��BNULL���w�肷��ƁApFeatureLevel�ŃT�|�[�g����Ă���ō��̋@�\���x�����Ԃ����
			NULL,									//���̃f�o�C�X�ŃT�|�[�g����Ă���@�\���x���̔z��ɂ���ŏ��̗v�f��\��D3D_FEATURE_LEVEL�ւ̃|�C���^�[��Ԃ�
			&context3D);						//�f�o�C�X �R���e�L�X�g��\��ID3D11DeviceContext�I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ�
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Direct3D::IsCreateBlendState()
{
	HRESULT hr;
	ATL::CComPtr<ID3D11BlendState> blendState = nullptr;
	D3D11_BLEND_DESC blendDesc;
	SecureZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	//�A���t�@�u�����f�B���O�̐ݒ�
	blendDesc.RenderTarget[0].BlendEnable = true;														//�u�����f�B���O��L��(�܂��͖���) 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;								//�ŏ���RGB�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;							//2�Ԗڂ�RGB�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;										//RGB�f�[�^�\�[�X�̑g�������@���`
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;									//�ŏ��̃A���t�@�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;								//2�Ԗڂ̃A���t�@�f�[�^�\�[�X���w��
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;								//�A���t�@�f�[�^�\�[�X�̑g�������@���`
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//�������݃}�X�N

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	hr = device3D->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr))
	{
		return false;
	}
	context3D->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	return true;
}

bool Direct3D::IsCreateD2D()
{
	HRESULT hr;
	D2D1_FACTORY_OPTIONS options;
#if defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	SecureZeroMemory(&options,sizeof(D2D1_FACTORY_OPTIONS));
	
	ATL::CComPtr<ID2D1Factory1> factory = nullptr;
	
	//Direct2D�t�@�N�g���̍쐬
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,		//�t�@�N�g���Ƃ��̃t�@�N�g���ɂ���č쐬����郊�\�[�X�̃X���b�h���f��
		options,												//�f�o�b�O���C���[�Ɏw�肳�ꂽ�ڍׂ̃��x��
		&factory);											//���̃��\�b�h���Ԃ����Ƃ��ɁA�V�����t�@�N�g���ւ̃|�C���^�[�̃A�h���X���i�[�����
	if (FAILED(hr))
	{
		return false;
	}
	ATL::CComPtr<IDXGIDevice> device = nullptr;
	//�ړI�̃C���^�[�t�F�[�X���g���邩�𓾂�
	hr = device3D.QueryInterface(&device);	
	if (FAILED(hr))
	{
		return false;
	}

	//Direct2D��DirectX11�ŘA�g�ł��邩�ǂ���
	hr = factory->CreateDevice(device, &device2D);
	if (FAILED(hr))
	{
		return false;
	}
	//Direct2DContext�̍쐬
	hr = device2D->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE, 
		&context2D);
	if (FAILED(hr))
	{
		return false;
	}
	//COM�I�u�W�F�N�g�̍쐬
	hr = textureFactory.CoCreateInstance(
		CLSID_WICImagingFactory, 
		nullptr, 
		CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		return false;
	}
	//DirectWrite�I�u�W�F�N�g���쐬
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory), 
		&textFactory);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void Direct3D::SetViewport()
{
	//�r���[�|�[�g�̍쐬
	D3D11_VIEWPORT viewPort;
	SecureZeroMemory(&viewPort,sizeof(viewPort));
	viewPort.Width = static_cast<float>(Engine::GetWindowWidth());
	viewPort.Height = static_cast<float>(Engine::GetWindowHeight());
	viewPort.MaxDepth = 1.0f;
	context3D->RSSetViewports(1, &viewPort);
}

void Direct3D::OnProceed(HWND, UINT message, WPARAM, LPARAM)
{
	//�E�B���h�E�T�C�Y���ύX���ꂽ�烁�b�Z�[�W�𑗂�
	if (message != WM_SIZE)
		return;

	if (Engine::GetWindowSize().x <= 0.0f || Engine::GetWindowSize().y <= 0.0f)
		return;

	SetViewport();
}
