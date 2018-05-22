#include "Mesh.h"
#include "Engine.h"
using namespace DirectX;

Mesh::Mesh():
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	rasterizerState(nullptr),
	pos(0,0,0),
	angle(0,0,0),
	scale(1,1,1)
{
	Engine::COMInitialize();

	material.Create(
		"cbuffer Camera : register(b0)"
		"{"
		"    matrix view;"
		"    matrix projection;"
		"};"
		"cbuffer Object : register(b1)"
		"{"
		"    matrix world;"
		"};"
		"Texture2D texture0 : register(t0);"
		"SamplerState sampler0 : register(s0);"
		"struct Vertex"
		"{"
		"    float4 position : POSITION;"
		"    float3 normal : NORMAL;"
		"    float2 uv : TEXCOORD;"
		"};"
		"struct Pixel"
		"{"
		"    float4 position : SV_POSITION;"
		"    float3 normal : NORMAL;"
		"    float2 uv : TEXCOORD;"
		"};"
		"Pixel VS(Vertex vertex)"
		"{"
		"    Pixel output;"
		"    output.position = mul(vertex.position, world);"
		"    output.position = mul(output.position, view);"
		"    output.position = mul(output.position, projection);"
		"    output.normal = mul(vertex.normal, (float3x3)world);"
		"    output.uv = vertex.uv;"
		"    return output;"
		"}"
		"float4 PS(Pixel pixel) : SV_TARGET"
		"{"
		"    float3 normal = normalize(pixel.normal);"
		"    float3 lightDirection = normalize(float3(0.55, -1.0, 0.5));"
		"    float3 lightColor = float3(1.0, 1.0, 1.0);"
		"    float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);"
		"    float3 diffuseIntensity = dot(-lightDirection, normal) * lightColor;"
		"    float3 ambientIntensity = lightColor * 0.2;"
		"    return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);"
		"}"
	);

	SetDrawMode(D3D11_CULL_NONE,D3D11_FILL_SOLID);

}

Mesh::~Mesh()
{

}


void Mesh::CreatePlane(Vec2 size, Vec3 offset, bool shouldClear, Vec3 leftDirection, Vec3 upDirection, Vec3 forwardDirection)
{
	if (shouldClear)
	{
		vertex.clear();
		index.clear();
	}

	leftDirection.Normalize();
	upDirection.Normalize();
	forwardDirection.Normalize();

	vertex.push_back(Vertex(
		leftDirection * -size.x + upDirection * size.y + offset,
		-forwardDirection, 
		Vec2(0.0f, 0.0f)));
	vertex.push_back(Vertex(
		leftDirection * size.x + upDirection * size.y + offset, 
		-forwardDirection, 
		Vec2(1.0f, 0.0f)));
	vertex.push_back(Vertex(
		leftDirection * -size.x + upDirection * -size.y + offset,
		-forwardDirection, 
		Vec2(0.0f, 1.0f)));
	vertex.push_back(Vertex(
		leftDirection * size.x + upDirection * -size.y + offset, 
		-forwardDirection, 
		Vec2(1.0f, 1.0f)));

	size_t indexOffset = vertex.size() - 4;
	index.push_back(indexOffset + 0);
	index.push_back(indexOffset + 1);
	index.push_back(indexOffset + 2);
	index.push_back(indexOffset + 3);
	index.push_back(indexOffset + 2);
	index.push_back(indexOffset + 1);

	Apply();
}

void Mesh::CreateCube(bool shouldClear)
{
	if (shouldClear)
	{
		vertex.clear();
		index.clear();
	}

	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -0.5f), false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 1.0f, 0.0f),  Vec3(0.0f, 0.0f, 1.0f));	// front
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, 0.5f),  false, Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),  Vec3(0.0f, 0.0f, -1.0f));	// back
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.5f, 0.0f, 0.0f),  false, Vec3(0.0f, 0.0f, 1.0f),  Vec3(0.0f, 1.0f, 0.0f),  Vec3(-1.0f, 0.0f, 0.0f));	// left
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(-0.5f, 0.0f, 0.0f), false, Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f),  Vec3(1.0f, 0.0f, 0.0f));	// right
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.5f, 0.0f),  false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 0.0f, 1.0f),  Vec3(0.0f, -1.0f, 0.0f));	// up
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, -0.5f, 0.0f), false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f));	// down

}


Material& Mesh::GetMaterial()
{
	return material;
}

void Mesh::SetDrawMode(D3D11_CULL_MODE cullingMode, D3D11_FILL_MODE fillMode)
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = fillMode;
	rasterizerDesc.CullMode = cullingMode;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerState.Release();
	Engine::GetDXDevice3D().CreateRasterizerState(&rasterizerDesc, &rasterizerState);
}

void Mesh::Apply()
{
	vertexBuffer.Release();
	if (vertex.size() > 0)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertex.size();
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
		vertexSubresourceData.pSysMem = vertex.data();
		Engine::GetDXDevice3D().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	}

	indexBuffer.Release();
	if (index.size() > 0)
	{
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(int) * index.size();
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
		indexSubresourceData.pSysMem = index.data();
		Engine::GetDXDevice3D().CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	}

	material.SetBuffer(1, &constant, sizeof(Constant));
}

void Mesh::Draw()
{
	if (vertexBuffer == nullptr)
	{
		return;
	}

	material.Attach();

	constant.world = XMMatrixTranspose(
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x)) *
		XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y)) *
		XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z)) *
		XMMatrixTranslation(pos.x, pos.y, pos.z)
	);

	Engine::GetDXContext3D().RSSetState(rasterizerState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Engine::GetDXContext3D().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);
	Engine::GetDXContext3D().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (indexBuffer == nullptr)
	{
		Engine::GetDXContext3D().Draw(vertex.size(), 0);
	}
	else
	{
		Engine::GetDXContext3D().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Engine::GetDXContext3D().DrawIndexed(index.size(), 0, 0);
	}
}
