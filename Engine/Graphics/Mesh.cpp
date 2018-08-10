#include "Mesh.h"
#include "../Engine.h"
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
	material.Load("Resource/Shader/mesh.hlsl");
	SetDrawMode(D3D11_CULL_NONE,D3D11_FILL_SOLID);

}

Mesh::~Mesh()
{

}

void Mesh::CreatePoint(Vec3 p1, Vec3 offset, Vec3 forwardDirection,bool shouldClear)
{
	if (shouldClear)
	{
		vertices.clear();
		indices.clear();
	}
	
	forwardDirection.Normalize();

	//1個目の点
	vertices.push_back(Vertex(
		p1  + offset,
		-forwardDirection,
		Vec2(0.0f, 1.0f)));


	size_t indexOffset = vertices.size() - 1;
	indices.push_back(indexOffset + 0);

	Apply();
}

void Mesh::CreatePlane(Vec2 size, Vec3 offset, bool shouldClear, Vec3 leftDirection, Vec3 upDirection, Vec3 forwardDirection)
{
	if (shouldClear)
	{
		vertices.clear();
		indices.clear();
	}

	leftDirection.Normalize();
	upDirection.Normalize();
	forwardDirection.Normalize();
	//平面の方程式 : ax+by+cz+d=0
	//n = (a,b,c)が平面の法線
	//dは原点からの符号付距離
	vertices.push_back(Vertex(
		leftDirection * -size.x + upDirection * size.y + offset,
		-forwardDirection, 
		Vec2(0.0f, 0.0f)));
	vertices.push_back(Vertex(
		leftDirection * size.x + upDirection * size.y + offset, 
		-forwardDirection, 
		Vec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(
		leftDirection * -size.x + upDirection * -size.y + offset,
		-forwardDirection, 
		Vec2(0.0f, 1.0f)));
	vertices.push_back(Vertex(
		leftDirection * size.x + upDirection * -size.y + offset, 
		-forwardDirection, 
		Vec2(1.0f, 1.0f)));

	size_t indexOffset = vertices.size() - 4;
	indices.push_back(indexOffset + 0);
	indices.push_back(indexOffset + 1);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 3);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 1);

	Apply();
}

void Mesh::CreateCube(bool shouldClear)
{
	if (shouldClear)
	{
		vertices.clear();
		indices.clear();
	}

	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -0.5f), false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 1.0f, 0.0f),  Vec3(0.0f, 0.0f, 1.0f));	// front
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, 0.5f),  false, Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),  Vec3(0.0f, 0.0f, -1.0f));	// back
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.5f, 0.0f, 0.0f),  false, Vec3(0.0f, 0.0f, 1.0f),  Vec3(0.0f, 1.0f, 0.0f),  Vec3(-1.0f, 0.0f, 0.0f));	// left
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(-0.5f, 0.0f, 0.0f), false, Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f),  Vec3(1.0f, 0.0f, 0.0f));	// right
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, 0.5f, 0.0f),  false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 0.0f, 1.0f),  Vec3(0.0f, -1.0f, 0.0f));	// up
	CreatePlane(Vec2(0.5f, 0.5f), Vec3(0.0f, -0.5f, 0.0f), false, Vec3(1.0f, 0.0f, 0.0f),  Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f));	// down

}


void Mesh::CreateSphere(float diameter, int tessellation, bool shouldClear)
{
	if (shouldClear)
	{
		vertices.clear();
		indices.clear();
	}

	int verticalSegments = tessellation;
	int horizontalSegments = tessellation * 2;

	float radius = diameter / 2;

	for (int i = 0; i <= verticalSegments; i++)
	{
		float v = float(i) / verticalSegments;

		float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos(&dy, &dxz, latitude);

		for (int j = 0; j <= horizontalSegments; j++)
		{
			float u = float(j) / horizontalSegments;

			float longitude = j * XM_2PI / horizontalSegments + XM_PI;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			Vec2 uv(u, v);
			XMFLOAT3 ntmp = { dx, dy, dz };
			XMVECTOR Nvec = XMLoadFloat3(&ntmp);
			XMVECTOR Xvec = XMVectorScale(-Nvec, radius);
			
			Vec3 nval;
			nval.x = ntmp.x;
			nval.y = ntmp.y;
			nval.z = ntmp.z;
			Vec3 val;
			val.x = XMVectorGetX(Xvec);
			val.y = XMVectorGetY(Xvec);
			val.z = XMVectorGetZ(Xvec);

			vertices.push_back(Vertex(Vec3(val), Vec3(-nval), Vec2(uv)));
		}
	}

	int stride = horizontalSegments + 1;

	for (int i = 0; i < verticalSegments; i++)
	{
		for (int j = 0; j <= horizontalSegments; j++)
		{
			int nextI = i + 1;
			int nextJ = (j + 1) % stride;

			indices.push_back(i * stride + j);
			indices.push_back(nextI * stride + j);
			indices.push_back(i * stride + nextJ);

			indices.push_back(i * stride + nextJ);
			indices.push_back(nextI * stride + j);
			indices.push_back(nextI * stride + nextJ);
		}
	}

	if (shouldClear)
		Apply();
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
	if (vertices.size() > 0)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
		vertexSubresourceData.pSysMem = vertices.data();
		Engine::GetDXDevice3D().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	}

	indexBuffer.Release();
	if (indices.size() > 0)
	{
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
		indexSubresourceData.pSysMem = indices.data();
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

	constant.world = XMMatrixTranspose(
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x)) *
		XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y)) *
		XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z)) *
		XMMatrixTranslation(pos.x, pos.y, pos.z)
	);

	material.Attach();

	Engine::GetDXContext3D().RSSetState(rasterizerState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Engine::GetDXContext3D().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);
	Engine::GetDXContext3D().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (indexBuffer == nullptr)
	{
		Engine::GetDXContext3D().Draw(vertices.size(), 0);
	}
	else
	{
		Engine::GetDXContext3D().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Engine::GetDXContext3D().DrawIndexed(indices.size(), 0, 0);
	}
}
void Mesh::Draw(DirectX::XMMATRIX rota)
{
	if (vertexBuffer == nullptr)
	{
		return;
	}

	constant.world = XMMatrixTranspose(
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		rota *
		XMMatrixTranslation(pos.x, pos.y, pos.z)
	);

	material.Attach();

	Engine::GetDXContext3D().RSSetState(rasterizerState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Engine::GetDXContext3D().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);
	Engine::GetDXContext3D().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (indexBuffer == nullptr)
	{
		Engine::GetDXContext3D().Draw(vertices.size(), 0);
	}
	else
	{
		Engine::GetDXContext3D().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Engine::GetDXContext3D().DrawIndexed(indices.size(), 0, 0);
	}
}
void Mesh::DrawPoint()
{
	if (vertexBuffer == nullptr)
	{
		return;
	}

	constant.world = XMMatrixTranspose(
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x)) *
		XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y)) *
		XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z)) *
		XMMatrixTranslation(pos.x, pos.y, pos.z)
	);

	material.Attach();

	Engine::GetDXContext3D().RSSetState(rasterizerState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Engine::GetDXContext3D().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);
	Engine::GetDXContext3D().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	if (indexBuffer == nullptr)
	{
		Engine::GetDXContext3D().Draw(vertices.size(), 0);
	}
	else
	{
		Engine::GetDXContext3D().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Engine::GetDXContext3D().DrawIndexed(indices.size(), 0, 0);
	}
}

