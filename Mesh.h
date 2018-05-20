#pragma once
#include "Utility.hpp"
#include "Material.h"
#include <DirectXMath.h>
#include <vector>

class Mesh
{
private:
	struct Constant
	{
		DirectX::XMMATRIX world;
	};

	Material material;
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer;
	ATL::CComPtr<ID3D11Buffer> indexBuffer;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState;
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;

	std::vector<Vertex> vertex;
	std::vector<UINT> index;
	Mesh();
	~Mesh();
	void CreatePlane(Vec2 size, Vec3 offset = Vec3(0.0f, 0.0f, 0.0f), bool shouldClear = true, Vec3 leftDirection = Vec3(1.0f, 0.0f, 0.0f), Vec3 upDirection = Vec3(0.0f, 1.0f, 0.0f), Vec3 forwardDirection = Vec3(0.0f, 0.0f, 1.0f));
	void CreateCube(bool shouldClear = true);
	Material& GetMaterial();
	void SetDrawMode(D3D11_CULL_MODE cullingMode, D3D11_FILL_MODE fillMode);
	void Apply();
	void Draw();
};

