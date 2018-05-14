#include "Camera.h"
using namespace DirectX;
Camera::Camera():
	renderTargetView(nullptr),
	depthStencilView(nullptr),
	renderTexture(nullptr),
	depthTexture(nullptr),
	constantBuffer(nullptr),
	pos(0,0,0),
	angles(0, 0, 0),
	color(0, 1, 1, 1),
	isDepthTest(false)
{
	Engine::COMInitialize();
	SetOrthographic(
		static_cast<float>(Engine::GetWindowSize().y),
		-D3D11_FLOAT32_MAX,
		D3D11_FLOAT32_MAX);
	System::AddProcedure(this);
	Create();
	
}


Camera::~Camera()
{
	System::RemoveProcedure(this);
}

void Camera::SetPerspective(float fieldOfView, float nearClip, float farClip)
{
	isPerspective = true;
	this->fieldOfView = fieldOfView;
	this->nearClip = nearClip;
	this->farClip = farClip;
	float aspectRatio = Engine::GetWindowWidth() / (float)Engine::GetWindowHeight();
	constant.projection = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearClip, farClip));
}

void Camera::SetOrthographic(float size, float nearClip, float farClip)
{
	isPerspective = false;
	this->size = size;
	this->nearClip = nearClip;
	this->farClip = farClip;
	float aspectRatio = Engine::GetWindowWidth() / (float)Engine::GetWindowHeight();
	constant.projection = XMMatrixTranspose(
		XMMatrixOrthographicLH(size * aspectRatio, size, nearClip, farClip));
}

void Camera::SetDepthTest(bool isDepthTest)
{
	this->isDepthTest = isDepthTest;
}

void Camera::Update()
{
	constant.view = XMMatrixTranspose(
		XMMatrixInverse(
			nullptr,
			XMMatrixRotationX(XMConvertToRadians(angles.x)) *
			XMMatrixRotationY(XMConvertToRadians(angles.y)) *
			XMMatrixRotationZ(XMConvertToRadians(angles.z)) *
			XMMatrixTranslation(pos.x, pos.y, pos.z)
		)
	);

	Engine::GetDXContext3D().UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
	Engine::GetDXContext3D().VSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().HSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().DSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().GSSetConstantBuffers(0, 1, &constantBuffer.p);
	Engine::GetDXContext3D().PSSetConstantBuffers(0, 1, &constantBuffer.p);

	float clearColor[4] = { color.x, color.y, color.z, color.w };
	Engine::GetDXContext3D().ClearRenderTargetView(renderTargetView, clearColor);

	if (isDepthTest)
	{
		Engine::GetDXContext3D().ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Engine::GetDXContext3D().OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}
	else
	{
		Engine::GetDXContext3D().OMSetRenderTargets(1, &renderTargetView.p, nullptr);

	}

}

bool Camera::Create()
{
	HRESULT hr;
	//��ɃN���A���Ă���
	renderTexture.Release();	

	//�o�b�N�o�b�t�@�[�ɃA�N�Z�X
	Engine::GetDXSwapChain().GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&renderTexture));

	renderTargetView.Release();

	//���\�[�X�f�[�^�ւ̃A�N�Z�X�p�Ƀ����_�[�^�[�Q�b�g�r���[���쐬
	hr = Engine::GetDXDevice3D().CreateRenderTargetView(
		renderTexture,				//ID3D11Resource�ւ̃|�C���^
		nullptr,					//D3D11_RENDER_TARGET_VIEW_DESC�ւ̃|�C���^�[
		&renderTargetView);			//ID3D11RenderTargetView�ւ̃|�C���^�[�̃A�h���X
	if (FAILED(hr))
	{
		MessageBox(NULL, "�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SecureZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	Engine::GetDXSwapChain().GetDesc(&swapChainDesc);
	depthTexture.Release();

	//2D�e�N�X�`���[�ɂ��ċL�q���܂�
	D3D11_TEXTURE2D_DESC textureDesc;
	SecureZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = static_cast<UINT>(Engine::GetWindowSize().x);	//�e�N�X�`���[�̕��ł� (�e�N�Z���P��)
	textureDesc.Height = static_cast<UINT>(Engine::GetWindowSize().y);	//�e�N�X�`���[�̍����ł� (�e�N�Z���P��)
	textureDesc.MipLevels = 1;											//�e�N�X�`���[���̃~�b�v�}�b�v ���x���̍ő吔
	textureDesc.ArraySize = 1;											//�e�N�X�`���[�z����̃e�N�X�`���[�̐��ł��B
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;						//�e�N�X�`���[�t�H�[�}�b�g
	textureDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;		//�s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	textureDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;	//�C���[�W�̕i�����x���ł��B�i���������قǁA�p�t�H�[�}���X�͒ቺ���܂��B�L���Ȕ͈͂�0����-1
	textureDesc.Usage = D3D11_USAGE_DEFAULT;							//�e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;					//�p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
	textureDesc.CPUAccessFlags = 0;										//������CPU�A�N�Z�X�̎�ނ��w�肷��t���O
	textureDesc.MiscFlags = 0;											//���̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O

	//2D�e�N�X�`���[�̔z����쐬
	hr = Engine::GetDXDevice3D().CreateTexture2D(
		&textureDesc,	//2D�e�N�X�`���[�̋L�q�ւ̃|�C���^�[
		nullptr,		//�T�u���\�[�X�̋L�q�̔z��ւ̃|�C���^�[
		&depthTexture);	//�쐬�����e�N�X�`���[�ւ̃|�C���^�[�̃A�h���X
	if (FAILED(hr))
	{
		MessageBox(NULL, "2D�e�N�X�`���[�z��̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	depthStencilView.Release();

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;	//���\�[�X�f�[�^�̃t�H�[�}�b�g

	if (swapChainDesc.SampleDesc.Count == 0)
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//���\�[�X�̃^�C�v.���\�[�X��2D�e�N�X�`���[�Ƃ��ăA�N�Z�X����܂��B
		depthStencilViewDesc.Texture2D.MipSlice = 0;		//�ŏ��Ɏg�p����~�b�v�}�b�v ���x���̃C���f�b�N�X
	}
	else
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;		//���\�[�X�̓}���`�T���v�����O���g�p����2D�e�N�X�`���[�Ƃ��ăA�N�Z�X����܂��B
	}

	//���\�[�X�f�[�^�ւ̃A�N�Z�X�p�ɐ[�x�X�e���V���r���[���쐬
	hr = Engine::GetDXDevice3D().CreateDepthStencilView(
		depthTexture,			 //�[�x�X�e���V���T�[�t�F�X�Ƃ��ċ@�\���郊�\�[�X�ւ̃|�C���^�[
		&depthStencilViewDesc,  //�[�x�X�e���V���r���[�̋L�q�ւ̃|�C���^�[
		&depthStencilView);		//ID3D11DepthStencilView�ւ̃|�C���^�[
	if (FAILED(hr))
	{
		MessageBox(NULL, "�[�x�X�e���V���r���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	constantBuffer.Release();
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC constantBufferDesc;
	SecureZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Constant));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	hr = Engine::GetDXDevice3D().CreateBuffer(
		&constantBufferDesc,
		nullptr, 
		&constantBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�[�x�X�e���V���r���[�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}
	return true;
}

void Camera::OnProceed(HWND, UINT message, WPARAM, LPARAM)
{
	//�E�B���h�E�T�C�Y���ύX���ꂽ�烁�b�Z�[�W�𑗂�
	if (message != WM_SIZE)
		return;

	if (Engine::GetWindowSize().x <= 0.0f || Engine::GetWindowSize().y <= 0.0f)
		return;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	Engine::GetDXSwapChain().GetDesc(&swapChainDesc);

	ATL::CComPtr<ID3D11RenderTargetView> nullRenderTarget = nullptr;
	ATL::CComPtr<ID3D11DepthStencilView> nullDepthStencil = nullptr;
	Engine::GetDXContext3D().OMSetRenderTargets(
		1, 
		&nullRenderTarget, 
		nullDepthStencil
	);
	renderTargetView.Release();
	depthStencilView.Release();
	renderTexture.Release();
	depthTexture.Release();
	Engine::GetDXContext3D().Flush();
	Engine::GetDXSwapChain().ResizeBuffers(
		swapChainDesc.BufferCount, 
		static_cast<UINT>(Engine::GetWindowSize().x), 
		static_cast<UINT>(Engine::GetWindowSize().y),
		swapChainDesc.BufferDesc.Format, 
		swapChainDesc.Flags);

	if (isPerspective)
		SetPerspective(fieldOfView, nearClip, farClip);
	else
		SetOrthographic(size, nearClip, farClip);

	Create();

}