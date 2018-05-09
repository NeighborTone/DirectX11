#pragma once
#include "Engine.h"
class Shader
{
private:
	ID3D11PixelShader*		 pPixelShader;
	ID3D11VertexShader*	 pVertexShader;
	ID3D11InputLayout*	     pVertexLayout;
	ID3DBlob*				     pCompiledShader;
	bool MakeVertexShader();
	bool MakePixelShader();
public:
	Shader();
	~Shader();
	bool CompileShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);
	bool SetLayOut();
	ID3D11PixelShader*   GetPixel();
	ID3D11VertexShader* GetVertex();
};

