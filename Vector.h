#pragma once

/*! @class Vec2
*   @brief 2次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さ測る機能があります
*/
class Vec2
{
public:
	float x, y;
	Vec2();
	Vec2(float x, float y);
	Vec2(const Vec2& v);
	~Vec2();

	/*!
	* @brief 内積を返します
	* @return float
	*/
	float Dot(Vec2& v);
	/*!
	* @brief 外積を返します
	* @return float
	*/
	float Cross(Vec2& v);
	/*!
	* @brief 長さを返します
	* @return float
	*/
	float Length();
	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec2
	*/
	Vec2&  Normalize();
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec2
	*/
	Vec2  GetNormalize() const;
	Vec2&	operator=(const Vec2& v);
	Vec2& operator+=(const Vec2& v);
	Vec2& operator-=(const Vec2& v);
	Vec2& operator*=(const Vec2& v);
	Vec2& operator/=(const Vec2& v);
	bool	operator==(const Vec2 &v) const;
	bool	operator!=(const Vec2 &v) const;

};

/*! @class Vec3
*   @brief 3次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さ測る機能があります
*/
class Vec3
{
public:
	float x, y, z;
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(const Vec3& v);
	~Vec3();

	/*!
	* @brief 内積を返します
	* @return float
	*/
	float Dot(Vec3& v);
	/*!
	* @brief 外積を返します
	* @return Vec3
	*/
	Vec3 Cross(Vec3& v);
	/*!
	* @brief 長さを返します
	* @return float
	*/
	float Length();
	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec3
	*/
	Vec3&  Normalize();
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec3
	*/
	Vec3  GetNormalize() const;
	Vec3&	operator=(const  Vec3& v);
	Vec3& operator+=(const Vec3& v);
	Vec3& operator-=(const Vec3& v);
	Vec3& operator*=(const Vec3& v);
	Vec3& operator/=(const Vec3& v);
	bool	operator==(const Vec3 &v) const;
	bool	operator!=(const Vec3 &v) const;

};