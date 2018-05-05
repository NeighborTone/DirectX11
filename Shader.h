#pragma once
#pragma warning( disable : 4005 )	//DirectXƒ}ƒNƒÄ’è‹`
#define RELEASE(p) {p->Release(); p = nullptr;}
#include <d3d11.h>
#include <D3DX10mesh.h>
#include <D3DX11async.h>
#include "Matrix.hpp"
#include <fstream>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"d3dx11.lib")
class Shader
{
private:

	ID3D11VertexShader * pVertex;
	ID3D11PixelShader*  pPixel;
	ID3D11InputLayout*  pLayout;
	ID3D11Buffer*         pMatrixBuf;
	char*						name;
	
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	bool CreateShader(ID3D11Device* device, HWND hwnd, const char* vsPath, const char* psPath, const char* vertexFuncName, const char* pixelFuncName);
	void OutputShaderError(ID3D10Blob* errorMes, HWND hwnd, const char* shaderPath);

public:

	Shader(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName);
	virtual ~Shader();
	virtual void Begin(ID3D11DeviceContext* context, int indexCount);
	virtual void End(ID3D11DeviceContext* context);
	bool SetShaderParameters(ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture);
	bool SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

	char* GetName();
	bool IsInit();

protected:
	bool						isInit;
	virtual bool Load(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName);

};

