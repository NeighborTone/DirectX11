#pragma once
#pragma warning( disable : 4005 )	//DirectXƒ}ƒNƒÄ’è‹`
#define RELEASE(p) {p->Release(); p = nullptr;}
#include <d3d11_1.h>
#include <D3DX11async.h>
#include "Matrix.hpp"
#include <string>
#include <fstream>

#pragma comment(lib,"d3dx11.lib")

class Shader
{
private:

	ID3D11VertexShader* pVertex;
	ID3D11PixelShader*  pPixel;
	ID3D11InputLayout*  pLayout;
	ID3D11Buffer*         pMatrixBuf;
	std::string				name;
	bool						isInit;
	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	bool CreateShader(ID3D11Device* device, HWND hwnd, const char* shaderPath);
	void OutputShaderError(ID3D10Blob* errorMes, HWND hwnd, const char* shaderPath);

public:

	Shader(ID3D11Device* device, HWND hwnd, const char* shaderPath);
	virtual ~Shader();
	virtual void Begin(ID3D11DeviceContext* context, int indexCount);
	virtual void End(ID3D11DeviceContext* context);
	bool SetShaderParameters(ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture);
	bool SetShaderParameters(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix proj);

	std::string GetName();
	bool IsInit();

protected:

	virtual bool Load(ID3D11Device* device, HWND hwnd, const std::string shaderPath);

};

