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
		pBlendState = nullptr;
		pVertexBuffer = nullptr;
		pVertexLayout = nullptr;
		pVertexShader = nullptr;
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
		if (pBlendState != nullptr)
		{
			RELEASE(pBlendState);
		}
		if (pVertexBuffer != nullptr)
		{
			RELEASE(pVertexBuffer);
		}
		if (pVertexLayout != nullptr)
		{
			RELEASE(pVertexLayout);
		}
		if (pVertexShader != nullptr)
		{
			RELEASE(pVertexShader);
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
		//デバイスとスワップチェーンの作成------------------------
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
		//------------------------------------------------

		//バックバッファーのレンダーターゲットビュー(RTV)を作成------
		ID3D11Texture2D *pBack;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);

		pDevice->CreateRenderTargetView(pBack, NULL, &pRTV);
		pBack->Release();
		//------------------------------------------------
		//デプスステンシルビュー(DSV)を作成--------------------
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
		//------------------------------------------------

		//ビューポートの設定----------------------------------
		D3D11_VIEWPORT vp;
		vp.Width = 640;
		vp.Height = 480;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1, &vp);
		//------------------------------------------------

		ID3DBlob *pCompiledShader = NULL;
		//バーテックスシェーダー作成
		MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&pVertexShader, &pCompiledShader);;
		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		//頂点インプットレイアウトを作成
		pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout);

		//ピクセルシェーダー作成
		MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&pPixelShader, &pCompiledShader);

		//コンスタントバッファー作成---------------------------------
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);
		//----------------------------------------------------

		//プリミティブの作成
		//バーテックスバッファー作成--------------------------------
		SimpleVertex vertices[] =
		{
			D3DXVECTOR3(-0.5f,-0.5f,0), D3DXVECTOR2(0,1), //頂点1,
			D3DXVECTOR3(-0.5f,0.5f,0),  D3DXVECTOR2(0,0),  //頂点2
			D3DXVECTOR3(0.5f,-0.5f,0),  D3DXVECTOR2(1,1),  //頂点3
			D3DXVECTOR3(0.5f,0.5f,0),   D3DXVECTOR2(1,0),  //頂点4
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * 4;	//頂点数を格納
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		//------------------------------------------------------


		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;
		pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
		
		Texture::GetInst()->Create();
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		
		pDevice->CreateBlendState(&BlendDesc, &pBlendState);


		return true;
	}

	void Device::Draw()
	{
		//レンダーターゲットビューとデプスステンシルビューをセット
		pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
		
		//加算合成を	有効にする	
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		pDeviceContext->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);

		//画面クリア
		//float ClearColor[4] = { 0.15f,0.55f,0.8f,1 };// クリア色作成　RGBAの順
		float ClearColor[4] = { 0,0,0,1 };
		pDeviceContext->ClearRenderTargetView(pRTV, ClearColor);//カラーバッファクリア
		pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア
		
		//3D変換-----------------------------------------------------------
		//3D変換はワールド変換、ビュー変換、プロジェクション変換の３つの変換を合成したもの
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
		//ワールドトランスフォーム
		D3DXMatrixIdentity(&world);

		//ビュートランスフォーム(カメラ)
		D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1.5f); //視点位置
		D3DXVECTOR3 vLookatPt(0,0,1.0f);//注視位置
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
		D3DXMatrixLookAtLH(&view, &vEyePt, &vLookatPt, &vUpVec);
		//プロジェクショントランスフォーム
		static float zoom = float(D3DX_PI / 4);

		D3DXMatrixPerspectiveFovLH(
			&proj,
			zoom,
			640.0f / 480.0f,
			0.1f,
			100.0f);
		//-----------------------------------------------------------------

		//使用するシェーダーのセット
		pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
		//シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//ワールド、カメラ、射影行列をシェーダーに渡す
			cb.mWVP = world * view * proj;
			D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			pDeviceContext->Unmap(pConstantBuffer, 0);
		}
		//このコンスタントバッファーをどのシェーダーで使うか
		pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//テクスチャーセット
		Texture::GetInst()->Draw();



		//バーテックスバッファーをセット
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


		//頂点インプットレイアウトをセット
		pDeviceContext->IASetInputLayout(pVertexLayout);
		//プリミティブ・トポロジーをセット
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//プリミティブをレンダリング
		pDeviceContext->Draw(4, 0);


		pSwapChain->Present(0, 0);//画面更新
	}
}
