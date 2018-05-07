#include "AnimatedSprite.h"
#include "Timer.h"	//デルタタイムを使用
#include "Engine.h"
AnimatedSprite::AnimatedSprite(float size, float framesPerSecond_, float animSpeed_, bool isLooping_) :
	Sprite(size),
	framesPerSecond((1000.0f/ framesPerSecond_) / 1000.0f),
	isLooping(isLooping_),
	animSpeed(animSpeed_),
	currentSpeed(0)
{

}


AnimatedSprite::~AnimatedSprite()
{
	
}

void AnimatedSprite::Create(ID3D11Device* device, ID3D11DeviceContext* context,Shader* shader, const char* texturePath)
{
	Sprite::Create(device, shader, texturePath, true);
	pContext = context;	//もしヌルだったら描画しない?
	currentFrame = 0;
	previousFrame = -1.0f;
	maxFrame = (float)pTex->GetWidth() / (float)pTex->GetHeight();
}

void AnimatedSprite::Update()
{
	if (maxFrame == 1.0f)
	{
		return;
	}
	if (currentFrame < maxFrame)
	{
		float dt = Timer::GetDeltaTime();
		currentSpeed += animSpeed * dt;
		if (currentSpeed > framesPerSecond)
		{
			++currentFrame;
			currentSpeed = 0.f;

			if (currentFrame >= maxFrame)
			{
				if (isLooping)
				{
					currentFrame = 0.0f;
				}
				else
				{
					currentFrame = maxFrame;
				}
			}
		}
	}
	if (currentFrame == previousFrame)
	{
		return;
	}

	//同じフレームであればバッファをロックしない
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexBuffer::VertexType* vertices = pVerBuf->GetVertices();
	vertices[0].uv.x = currentFrame / maxFrame;
	vertices[0].uv.y = 1.0f;

	vertices[1].uv.x = currentFrame / maxFrame;
	vertices[1].uv.y = 0.0f;

	vertices[2].uv.x = (currentFrame + 1.0f) / maxFrame;
	vertices[2].uv.y = 0.0f;

	vertices[3].uv.x = (currentFrame + 1.0f) / maxFrame;
	vertices[3].uv.y = 1.0f;

	//頂点バッファをロック
	HRESULT result = pContext->Map(pVerBuf->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	VertexBuffer::VertexType* verticesPtr = (VertexBuffer::VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexBuffer::VertexType) * pVerBuf->GetVertexCount());

	pContext->Unmap(pVerBuf->GetVertexBuffer(), 0);

	//前のフレームを更新する
	previousFrame = currentFrame;
}

void AnimatedSprite::Render(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	Engine::GetInst()->GetGraphics()->EnableAlphaBlending(true);
	Sprite::Render(context, world, view, proj);
	Engine::GetInst()->GetGraphics()->EnableAlphaBlending(false);
}
