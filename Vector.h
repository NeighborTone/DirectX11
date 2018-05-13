#pragma once

/*! @class Vec2
*   @brief 2�����x�N�g���������܂�
*   @detail ���ρA�O�ρA���K���A��������@�\������܂�
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
	* @brief ���ς�Ԃ��܂�
	* @return float
	*/
	float Dot(Vec2& v);
	/*!
	* @brief �O�ς�Ԃ��܂�
	* @return float
	*/
	float Cross(Vec2& v);
	/*!
	* @brief ������Ԃ��܂�
	* @return float
	*/
	float Length();
	/*!
	* @brief �������g�𐳋K�������l��Ԃ��܂�
	* @return Vec2
	*/
	Vec2&  Normalize();
	/*!
	* @brief �������g�𐳋K�������l�̃R�s�[��Ԃ��܂��B���g�̒l�͕ς��܂���
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
*   @brief 3�����x�N�g���������܂�
*   @detail ���ρA�O�ρA���K���A��������@�\������܂�
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
	* @brief ���ς�Ԃ��܂�
	* @return float
	*/
	float Dot(Vec3& v);
	/*!
	* @brief �O�ς�Ԃ��܂�
	* @return Vec3
	*/
	Vec3 Cross(Vec3& v);
	/*!
	* @brief ������Ԃ��܂�
	* @return float
	*/
	float Length();
	/*!
	* @brief �������g�𐳋K�������l��Ԃ��܂�
	* @return Vec3
	*/
	Vec3&  Normalize();
	/*!
	* @brief �������g�𐳋K�������l�̃R�s�[��Ԃ��܂��B���g�̒l�͕ς��܂���
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