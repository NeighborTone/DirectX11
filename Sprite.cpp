#include "Sprite.h"
#include "Engine.h"

void Sprite::Initialize()
{
	Engine::COMInitialize();

	pos = Vec3(0.0f, 0.0f, 0.0f);
	angle = Vec3(0.0f, 0.0f, 0.0f);
	scale = Vec3(1.0f, 1.0f, 1.0f);
	color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	mesh.GetMaterial().Create(
		"cbuffer Camera : register(b0)"
		"{"
		"    matrix view;"
		"    matrix projection;"
		"};"
		"cbuffer Object : register(b1)"
		"{"
		"    matrix world;"
		"};"
		"cbuffer Sprite : register(b2)"
		"{"
		"    float4 color;"
		"};"
		"Texture2D texture0 : register(t0);"
		"SamplerState sampler0 : register(s0);"
		"struct Vertex"
		"{"
		"    float4 position : POSITION;"
		"    float2 uv : TEXCOORD;"
		"};"
		"struct Pixel"
		"{"
		"    float4 position : SV_POSITION;"
		"    float2 uv : TEXCOORD;"
		"};"
		"Pixel VS(Vertex vertex)"
		"{"
		"    Pixel output;"
		"    output.position = mul(vertex.position, world);"
		"    output.position = mul(output.position, view);"
		"    output.position = mul(output.position, projection);"
		"    output.uv = vertex.uv;"
		"    return output;"
		"}"
		"float4 PS(Pixel pixel) : SV_TARGET"
		"{"
		"    float4 textureColor = texture0.Sample(sampler0, pixel.uv);"
		"    if (textureColor.a <= 0)"
		"        discard;"
		"    return textureColor * color;"
		"}"
	);

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

Material & Sprite::GetMaterial()
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
