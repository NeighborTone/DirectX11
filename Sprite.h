#pragma once
#include "Texture.h"
#include "VertexBuffer.h"

class Sprite
{
public:
	Sprite(float size);
	virtual ~Sprite();
	virtual void Create(ID3D11Device* device, Shader* shader, const char* texturePath,bool isAnim = false);
	virtual void Update(){};
	virtual void Render(ID3D11DeviceContext* context,D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

protected:
	VertexBuffer* pVerBuf;
	Texture* pTex;
	Shader* pShader;
	float size;


};

