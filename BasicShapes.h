#pragma once
#include "GeometricPrimitive.h"
#include "Utility.hpp"
#include "Camera.h"
#include <memory>
#pragma comment(lib,"DirectXTK.lib")


class BallShape
{
private:
	struct Constant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	std::unique_ptr<DirectX::GeometricPrimitive> ball;
	Constant constant;

public:
	Vec3 pos;
	Vec3 angle;
	float r;
	BallShape();
	~BallShape();

	void Create(float r,size_t tessellation,Camera& camera);
	void Draw(Camera& camera);
};

