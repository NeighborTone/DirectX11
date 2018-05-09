#include "Shader.h"
#include "SystemHelp.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

Shader::Shader():
	pPixelShader(nullptr),
  pVertexShader(nullptr),
  pVertexLayout(nullptr),
pCompiledShader(nullptr)
{

}

Shader::~Shader()
{
	SystemHelp::SafeRelease(pPixelShader);
	SystemHelp::SafeRelease(pVertexShader);
	SystemHelp::SafeRelease(pVertexLayout);
	SystemHelp::SafeRelease(pCompiledShader);

}

bool Shader::CompileShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	auto ge = Engine::GetEngine();
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(
		szFileName,
		NULL, 
		NULL, 
		szFuncName,
		szProfileName, 
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 
		0, 
		NULL, 
		ppBlob, 
		&pErrors, 
		NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return false;
	}
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	//バッファを作る
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		ge->GetGraphic()->GetDX()->GetDevice()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
	}
	return true;
}

bool Shader::MakeVertexShader()
{
	if (CompileShader("Shader.hlsl", "VS", "vs_5_0", (void**)&pVertexShader, &pCompiledShader))
	{
		return true;
	}
	return false;
}

bool Shader::MakePixelShader()
{
	if (CompileShader("Shader.hlsl", "VS", "vs_5_0", (void**)&pPixelShader, &pCompiledShader))
	{
		return true;
	}
	return false;
}

bool Shader::SetLayOut()
{
	HRESULT hr;
	if (!MakeVertexShader())
	{
		return false;
	}
	auto ge = Engine::GetEngine();
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	hr = ge->GetGraphic()->GetDX()->GetDevice()->CreateInputLayout(
		layout, 
		numElements, 
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), 
		&pVertexLayout);
	if (FAILED(hr))
	{

		return false;
	}
	if (!MakePixelShader())
	{
		return false;
	}
	return true;
}

ID3D11PixelShader * Shader::GetPixel()
{
	return pPixelShader;
}

ID3D11VertexShader * Shader::GetVertex()
{
	return pVertexShader;
}
