#include "Sprite.h"
#include "../Engine.h"

void Sprite::Initialize()
{
	Engine::COMInitialize();

	pos = Vec3(0.0f, 0.0f, 0.0f);
	angle = Vec3(0.0f, 0.0f, 0.0f);
	scale = Vec3(1.0f, 1.0f, 1.0f);
	color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	mesh.GetMaterial().Load("Resource/Shader/sprite.hlsl");
	mesh.GetMaterial().SetBuffer(2, &color, sizeof(Float4));
	mesh.SetDrawMode(D3D11_CULL_NONE,D3D11_FILL_SOLID);
}

Sprite::Sprite()
{
	Initialize();
}

Sprite::Sprite(const char* const filePath)
{
	Initialize();
	Load(filePath);
}

Sprite::Sprite(const BYTE* const buffer, int width, int height)
{
	Initialize();
	Create(buffer,width,height);
}


Sprite::~Sprite()
{

}

void Sprite::Load(const char* const filePath)
{
	texture.Load(filePath);

	mesh.GetMaterial().SetTexture(0, &texture);

	SetPivot(Vec2(0,0));
}

void Sprite::Create(const BYTE* const buffer, int width, int height)
{
	texture.Create(buffer, width, height);

	mesh.GetMaterial().SetTexture(0, &texture);

	SetPivot(Vec2(0,0));
}

DirectX::XMINT2 Sprite::GetSize() const
{
	return texture.GetSize();
}

void Sprite::SetPivot(Vec2 pivot)
{
	Vec2 textureSize(static_cast<float>(texture.GetSize().x), static_cast<float>(texture.GetSize().y));
	Vec2 offset = textureSize / 2.0f * -pivot;
	
	mesh.CreatePlane(textureSize / 2.0f, Vec3(offset.x, offset.y, 0.0f));
	
}

Material& Sprite::GetMaterial()
{
	return mesh.GetMaterial();
}

void Sprite::Draw()
{
	mesh.pos = pos;
	mesh.angle = angle;
	mesh.scale = scale;
	mesh.Draw();
}

bool Sprite::HitSprite(Sprite& s2)
{
	Vec3 leftTop1 = pos.OffSetCopy(-(GetSize().x / 2.0f), GetSize().y / 2.0f, pos.z);			//ç∂è„
	Vec3 leftTop2 = s2.pos.OffSetCopy(-(s2.GetSize().x / 2.0f), s2.GetSize().y / 2.0f, s2.pos.z);
	Vec3 buttom1 = pos.OffSetCopy(GetSize().x / 2.0f, -(GetSize().y / 2.0f), pos.z);			//âEâ∫
	Vec3 buttom2 = s2.pos.OffSetCopy(s2.GetSize().x / 2.0f, -(s2.GetSize().y / 2.0f), s2.pos.z);

	if (leftTop1.x < buttom2.x &&
		leftTop2.x < buttom1.x &&
		leftTop1.y > buttom2.y &&
		leftTop2.y > buttom1.y)
		return true;

	return false;
}

bool Sprite::HitCircle(Sprite& c2)
{
	if ((pos.x - c2.pos.x) * (pos.x - c2.pos.x) +
		((pos.y - c2.pos.y) * (pos.y - c2.pos.y)) <=
		(GetSize().x / 2 + GetSize().y / 2) * (c2.GetSize().x / 2 + c2.GetSize().y / 2))
		return true;
	return false;
}
