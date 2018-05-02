#include "Device.h"

namespace DX11
{
	Device::Device()
	{
		pDevice = nullptr;
		pDeviceContext = nullptr;
		pSwapChain = nullptr;
		pRTV = nullptr;
		pDS = nullptr;
		pDSV = nullptr;
	}

	Device::~Device()
	{
		if (pDevice != nullptr)
		{
			RELEASE(pDevice);
		}
		if (pDeviceContext != nullptr)
		{
			RELEASE(pDeviceContext);
		}
		if (pSwapChain != nullptr)
		{
			RELEASE(pSwapChain);

		}
		if (pRTV != nullptr)
		{
			RELEASE(pRTV);

		}
		if (pDS != nullptr)
		{
			RELEASE(pDS);

		}
		if (pDSV != nullptr)
		{
			RELEASE(pDSV);

		}
	}
	bool Device::MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
	{
		ID3DBlob *pErrors = NULL;
		if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
		{
			char*p = (char*)pErrors->GetBufferPointer();
			MessageBoxA(0, p, 0, MB_OK);
			return false;
		}
		char szProfile[3] = { 0 };
		memcpy(szProfile, szProfileName, 2);
		if (strcmp(szProfile, "vs") == 0)//Vertex Shader
		{
			pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		}
		if (strcmp(szProfile, "ps") == 0)//Pixel Shader
		{
			pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		}
		if (strcmp(szProfile, "gs") == 0)//Geometry Shader
		{
			pDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		}
		if (strcmp(szProfile, "hs") == 0)//Hull Shader
		{
			pDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		}
		if (strcmp(szProfile, "ds") == 0)//Domain Shader
		{
			pDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		}
		if (strcmp(szProfile, "cs") == 0)//Compute Shader
		{
			pDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
		}
		return true;
	}
	bool Device::InitDirect3D(System& win)
	{
		//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		DXGI_SWAP_CHAIN_DESC sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = 640;
		sd.BufferDesc.Height = 480;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = win.GetWindow();
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE, 
			NULL,
			0,
			&pFeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			pFeatureLevel,
			&pDeviceContext);

		//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
		ID3D11Texture2D *pBack;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);

		pDevice->CreateRenderTargetView(pBack, NULL, &pRTV);
		pBack->Release();

		//�f�v�X�X�e���V���r���[(DSV)���쐬
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = 640;
		descDepth.Height = 480;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		pDevice->CreateTexture2D(&descDepth, NULL, &pDS);

		pDevice->CreateDepthStencilView(pDS, NULL, &pDSV);

		//�r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = 640;
		vp.Height = 480;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1, &vp);

		ID3DBlob *pCompiledShader = NULL;
		//�o�[�e�b�N�X�V�F�[�_�[�쐬
		MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&pVertexShader, &pCompiledShader);;
		//���_�C���v�b�g���C�A�E�g���`	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		//���_�C���v�b�g���C�A�E�g���쐬
		pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout);

		//�s�N�Z���V�F�[�_�[�쐬
		MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&pPixelShader, &pCompiledShader);

		//�R���X�^���g�o�b�t�@�[�쐬
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

		//�g���C�A���O���쐬
		//�o�[�e�b�N�X�o�b�t�@�[�쐬
		SimpleVertex vertices[] =
		{
			D3DXVECTOR3(-0.5,-0.5,0),//���_1	
			D3DXVECTOR3(-0.5,0.5,0), //���_2
			D3DXVECTOR3(0.5,-0.5,0),  //���_3
		};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;
		pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);

		

		return true;
	}

	void Device::Draw()
	{
		//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
		pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
		//��ʃN���A
		float ClearColor[4] = { 0.15f,0.55f,0.8f,1 };// �N���A�F�쐬�@RGBA�̏�
		pDeviceContext->ClearRenderTargetView(pRTV, ClearColor);//�J���[�o�b�t�@�N���A
		pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A

		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Proj;
		//���[���h�g�����X�t�H�[��
		D3DXMatrixIdentity(&World);
		// �r���[�g�����X�t�H�[��
		D3DXVECTOR3 vEyePt(0.0f, 1.0f, -2.0f); //���_�ʒu
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
		D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
		// �v���W�F�N�V�����g�����X�t�H�[��
		D3DXMatrixPerspectiveFovLH(
			&Proj,
			FLOAT(D3DX_PI / 4),
			640.0f / 480.0f,
			0.1f,
			100.0f);

		//�g�p����V�F�[�_�[�̃Z�b�g
		pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//���[���h�A�J�����A�ˉe�s���n��
			cb.mWVP = World * View*Proj;
			D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			pDeviceContext->Unmap(pConstantBuffer, 0);
		}
		//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
		pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


		//���_�C���v�b�g���C�A�E�g���Z�b�g
		pDeviceContext->IASetInputLayout(pVertexLayout);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//�v���~�e�B�u�������_�����O
		pDeviceContext->Draw(3, 0);


		pSwapChain->Present(0, 0);//��ʍX�V
	}
}
