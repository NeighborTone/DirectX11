#include "Vector.h"
#include <math.h>

Vec2::Vec2():
	x(0),
	y(0)
{
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const Vec2& v)
{
	x = v.x;
	y = v.y;
}

Vec2::~Vec2()
{

}

float Vec2::Dot(Vec2& v)
{
	return x * v.x + y * v.y;
}

float Vec2::Cross(Vec2& v)
{
	return x * v.y - v.x * y;
}

float Vec2::Length()
{
	return sqrtf(x * x + y * y);
}

Vec2& Vec2::Normalize()
{
	float tmp = Length();
	x /= tmp;
	y /= tmp;
	return *this;
}

Vec2 Vec2::GetNormalize() const
{
	Vec2 result = *this;
	result.Normalize();
	return result;
}

Vec2&	Vec2::operator=(const Vec2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}
Vec2& Vec2::operator+=(const Vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}
Vec2& Vec2::operator*=(const Vec2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}
Vec2& Vec2::operator/=(const Vec2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}
bool	Vec2::operator==(const Vec2 &v) const
{
	return x == v.x && y == v.y;
}
bool	Vec2::operator!=(const Vec2 &v) const
{
	return !(*this == v);
}

Vec3::Vec3():
	x(0),
	y(0),
	z(0)
{
}

Vec3::Vec3(float x, float y,float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3::Vec3(const Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
Vec3::~Vec3()
{

}

float Vec3::Dot(Vec3& v)
{
	return x * v.x + y *v.y + z * v.z;
}

Vec3 Vec3::Cross(Vec3& v)
{
	Vec3 result;
	result.x = y*v.z - z*v.y;
	result.y = z*v.x - x*v.z;
	result.z = x*v.y - y*v.x;
	return result;
}

float Vec3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

Vec3& Vec3::Normalize()
{
	float tmp = Length();
	x /= tmp;
	y /= tmp;
	z /= tmp;
	return *this;
}

Vec3 Vec3::GetNormalize() const
{
	Vec3 result = *this;
	result.Normalize();
	return result;
}

Vec3&	Vec3::operator=(const  Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
Vec3& Vec3::operator+=(const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vec3& Vec3::operator*=(const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}
Vec3& Vec3::operator/=(const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;

}
bool Vec3::operator==(const Vec3 &v) const
{
	return (x == v.x && y == v.y && v.z == z);
}
bool Vec3::operator!=(const Vec3 &v) const
{
	return !(*this == v);
}


Float4::Float4():
	x(0),
	y(0),
	z(0),
	w(0)
{
}
Float4::Float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Float4::Float4(const Float4& f4)
{
	x = f4.x;
	y = f4.y;
	z = f4.z;
	w = f4.w;
}
Float4::~Float4()
{

}
Float4&	Float4::operator=(const Float4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}
Float4& Float4::operator+=(const Float4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
Float4& Float4::operator-=(const Float4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}
Float4& Float4::operator*=(const Float4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}
Float4& Float4::operator/=(const Float4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}
bool	Float4::operator==(const Float4 &v) const
{
	return (x == v.x && y == v.y && v.z == z && w == v.w);
}
bool	Float4::operator!=(const Float4 &v) const
{
	return !(*this == v);
}