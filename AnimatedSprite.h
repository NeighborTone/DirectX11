#pragma once
#include "Sprite.h"
class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite();
	virtual ~AnimatedSprite();
	void Create(ID3D11Device* device, Shader* shader, const char* texturePath, bool isAnim = false);
	void Update();
	void Render(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

};

