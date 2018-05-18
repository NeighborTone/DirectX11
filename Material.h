#pragma once
#include <string>
#include "Texture.h"
class Material
{
public:
	struct ConstantBuffer
	{
		ConstantBuffer() :ptr(nullptr), buffer(nullptr) {};
		void* ptr;
		ATL::CComPtr<ID3D11Buffer> buffer;
	};
private:
	Texture* textures[10];
	ConstantBuffer constbuff[10];
	ATL::CComPtr<ID3D11VertexShader>vertexShader;
	ATL::CComPtr<ID3D11PixelShader>pixelShader;
	ATL::CComPtr<ID3D11InputLayout>inputLayout;

	void 	Initialize();
	static void 	CompileShader(const std::string &source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out);
public:
	Material();
	~Material();
	Material(const char* const filePath);
	Material(const std::string &source);
	void 	Load(const char* const filePath);
	void 	Create(const std::string &source);
	void 	SetBuffer(int slot, void* cbuffer, size_t size);
	void 	SetTexture(int slot, Texture *texture);
	void 	Attach();

};

