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
//!@brief	�ό`������R���|�[�l���g
//!@brief	�ʒu�E��]�E�g�k
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
//!@brief	�X�v���C�g��2D��Ԃň���
//!@detail	�摜�̃t�@�C���p�X�͓��{��ł͓ǂݍ��ݕs��
//!@detail	�摜�̉�]�ʒu�̓f�t�H���g�Œ��S
//--------------------------------------------------------------------
class Sprite2D
{
public:
	//!@brief	�R���X�g���N�^
	//!@param[in]	filePath	�g�p����摜�t�@�C���p�X
	Sprite2D(const std::string& filePath);

	//!@brief	�f�X�g���N�^
	~Sprite2D();
public:
	//!@brief	�`��
	//!@param[in]	draw	�`��ʒu(x,y,width,height)
	//!@param[in]	src �\�[�X�͈̔�(x,y,width,height)
	//!@param[in]	IsCenter	true: draw�摜�̒��S���W false: ����̍��W
	//!@param[in]	color	�F(r,g,b,a)
	//!@param[in]	layer	�摜�̕`��\�[�h(0.0f �` 1.0f)
	void	Draw(const Math::Box2D& draw, const Math::Box2D& src, const bool IsCenter = true, const Math::Color& color = Math::Color(1, 1, 1, 1), float layer = 1.f);

	//!@brief	��]�ʂ̒���
	//!@param[in]	rotate	��]��(�x���@)
	void	SetRotation(const float rotate);
private:
	DirectX::SpriteBatch*	 spriteBatch;
	ID3D11ShaderResourceView*	textureResourceView;
	ID3D11Resource*	resource;
	ATL::CComPtr<ID3D11SamplerState> samplerState;
	float rotate;
	DirectX::XMFLOAT2	pivot;

};



