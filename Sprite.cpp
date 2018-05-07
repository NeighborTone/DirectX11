#include "Sprite.h"
#include "SystemDefs.h"
Sprite::Sprite(float size)
{
	pVerBuf = nullptr;
	pTex = nullptr;
	this->size = size;

}

Sprite::~Sprite()
{
	Memory::SafeDelete(pVerBuf);
	Memory::SafeDelete(pTex);
}

void Sprite::Create(ID3D11Device* device, Shader* shader, const char* texturePath, bool isWriteAble)
{
	pShader = shader;
	pVerBuf = new VertexBuffer();
	pVerBuf->Create(device, shader, size, isWriteAble);

	if (texturePath != nullptr)
	{
		pTex = new Texture();
		pTex->Create(device, texturePath);

	}

}

void Sprite::Render(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	if (pTex)
	{
		pShader->SetShaderParameters(context, pTex->GetTexture());
		pShader->SetShaderParameters(context, world, view, proj);

		pVerBuf->Draw(context);
	}
}


