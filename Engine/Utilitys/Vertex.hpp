#pragma once
#include "Vec.hpp"
/*! @class Vertex
*   @brief 頂点用の構造体です
*   @detail 位置情報、法線、UV値を格納できます
*/
struct Vertex
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec3 tangent;
	unsigned int blendIndices[8] = { 999, 999, 999, 999, 999, 999, 999, 999 };
	float blendWeights[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	Vertex() :
		pos(0, 0, 0),
		normal(0, 0, 0),
		uv(0, 0),
		tangent(0, 0, 0)
	{}
	Vertex(Vec3&& pos, Vec3&& normal, Vec2&& uv)
	{
		this->pos = pos;
		this->normal = normal;
		this->uv = uv;
	}
};