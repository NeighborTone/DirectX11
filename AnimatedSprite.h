#pragma once
#include "Sprite.h"
class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(float size, float framesPerSecond, float animSpeed = 1.0f, bool isLooping = true);
	virtual ~AnimatedSprite();
	void Create(ID3D11Device* device, ID3D11DeviceContext* conrext, Shader* shader, const char* texturePath);
	void Update();
	void Render(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
private:
	ID3D11DeviceContext* pContext;
	float currentFrame;
	float previousFrame;
	float maxFrame;
	float animSpeed;
	float currentSpeed;
	float framesPerSecond;
	bool isLooping;
};

