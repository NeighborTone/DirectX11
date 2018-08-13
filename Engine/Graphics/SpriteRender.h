#pragma once
#include <string>
#include <iostream>
#include "../Engine.h"
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <Effects.h>
#include "../Utilitys/Utility.hpp"
#ifdef DEBUG
#pragma comment(lib,"Engine/DirectXTK/lib/Debug/DirectXTK.lib")
#else
#pragma comment(lib,"Engine/DirectXTK/lib/Release/DirectXTK.lib")
#endif



//----------------------------------------------------------------------------
//!@class		Transform
//!@brief	変形させるコンポーネント
//!@brief	位置・回転・拡縮
//----------------------------------------------------------------------------
namespace Math
{
	class Box2D : public RECT
	{
	public:
		Box2D() {}
		Box2D(const Box2D& box2D)
		{
			x = box2D.x;
			y = box2D.y;
			width = box2D.width;
			height = box2D.height;
			this->left = (LONG)x;
			this->top = (LONG)y;
			this->bottom = (LONG)y + (LONG)height;
			this->right = (LONG)x + (LONG)width;
		}
		Box2D(float x, float y, float w, float h)
		{
			this->x = x;
			this->y = y;
			this->width = w;
			this->height = h;

			this->left = (LONG)x;
			this->top = (LONG)y;
			this->bottom = (LONG)y + (LONG)height;
			this->right = (LONG)x + (LONG)width;
		}

	public:
		float x, y;
		float width;
		float height;
	};

	class Color
	{
	public:
		Color() {};
		Color(float red, float green, float blue, float alpha)
		{
			color.r = red;
			color.g = green;
			color.b = blue;
			color.a = alpha;
		}
		Color(const Color& color)
		{
			this->color.r = color.color.r;
			this->color.g = color.color.g;
			this->color.b = color.color.b;
			this->color.a = color.color.a;
		}
		const float	GetRed() const
		{
			return color.r;
		}
		const float GetGreen() const
		{
			return color.g;
		}
		const float GetBlue() const
		{
			return color.b;
		}
		const float GetAlpha() const
		{
			return color.a;
		}

	private:
		Float4 color;
	};
}


//--------------------------------------------------------------------
//!@class		Sprite2D
//!@brief	スプライトを2D空間で扱う
//!@detail	画像のファイルパスは日本語では読み込み不可
//!@detail	画像の回転位置はデフォルトで中心
//--------------------------------------------------------------------
class Sprite2D
{
public:
	//!@brief	コンストラクタ
	//!@param[in]	filePath	使用する画像ファイルパス
	Sprite2D(const std::string& filePath);

	//!@brief	デストラクタ
	~Sprite2D();
public:
	//!@brief	描画
	//!@param[in]	draw	描画位置(x,y,width,height)
	//!@param[in]	src ソースの範囲(x,y,width,height)
	//!@param[in]	IsCenter	true: draw画像の中心座標 false: 左上の座標
	//!@param[in]	color	色(r,g,b,a)
	//!@param[in]	layer	画像の描画ソード(0.0f ～ 1.0f)
	void	Draw(const Math::Box2D& draw, const Math::Box2D& src, const bool IsCenter = true, const Math::Color& color = Math::Color(1, 1, 1, 1), float layer = 1.f);

	//!@brief	回転量の調節
	//!@param[in]	rotate	回転量(度数法)
	void	SetRotation(const float rotate);
private:
	DirectX::SpriteBatch*	 spriteBatch;
	ID3D11ShaderResourceView*	textureResourceView;
	ID3D11Resource*	resource;
	ATL::CComPtr<ID3D11SamplerState> samplerState;
	float rotate;
	DirectX::XMFLOAT2	pivot;

};



