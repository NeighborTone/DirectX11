#pragma once
#include "Shader.h"

#include <d3d11.h>
#include <D3DX10math.h>

class VertexBuffer
{

public:
	struct VertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

	VertexBuffer();
	~VertexBuffer();
	bool Create(ID3D11Device* device, Shader* shader, float size, bool writeable = false);
	void Draw(ID3D11DeviceContext* deviceContext);
	VertexType* GetVertices();
	ID3D11Buffer* GetVertexBuffer();
	int GetIndexCount();
	int GetVertexCount();

private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	int vertexCount;
	int indexCount;
	VertexType* pVertices;
	Shader* pShader;

};

