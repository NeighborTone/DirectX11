#pragma once
#pragma warning (disable : 4458)	//thisを使うため消す
template <typename T>
/*! @class Vec2
*   @brief 2次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さを測る機能があります
*/
class TemplateVec2
{
public:
	T x, y;
	TemplateVec2():
	x(0),
	y(0)
	{};
	TemplateVec2(const T x, const T y)
	{
		this->x = x;
		this->y = y;
	};
	TemplateVec2(const TemplateVec2& v)
	{
		x = v.x;
		y = v.y;
	}
	~TemplateVec2() {};

	/*!
	* @brief 内積を返します
	* @return 内積
	*/
	T Dot(TemplateVec2& v)
	{
		return x * v.x + y * v.y;
	}

	/*!
	* @brief 外積を返します
	* @return 外積
	*/
	T Cross(TemplateVec2& v)
	{
		return x * v.y - v.x * y;
	}

	/*!
	* @brief 長さを返します
	* @return 長さ
	*/
	T Length()
	{
		return sqrtf(x * x + y * y);
	}
	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec2
	*/
	TemplateVec2&  Normalize()
	{
		T tmp = Length();
		x /= tmp;
		y /= tmp;
		return *this;
	}
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec2
	*/
	TemplateVec2  GetNormalize() const
	{
		TemplateVec2 result = *this;
		result.Normalize();
		return result;
	}

	TemplateVec2 operator+() const
	{
		return TemplateVec2(*this);
	}

	TemplateVec2 operator-() const
	{
		return TemplateVec2(-x, -y);
	}

	TemplateVec2 operator+(const TemplateVec2& v)
	{
		TemplateVec2 ret(*this);
		ret += v;
		return ret;
	}

	 TemplateVec2 operator+(const T& t)
	{
		 TemplateVec2 ret(*this);
		 ret += t;
		 return ret;
	}

	TemplateVec2 operator-(const TemplateVec2& v)
	{
		TemplateVec2 ret(*this);
		ret -= v	;
		return ret;
	}

	TemplateVec2 operator-(const T& t)
	{
		TemplateVec2 ret(*this);
		ret -= t;
		return ret;
	}

	TemplateVec2 operator*(const TemplateVec2& v)
	{
		TemplateVec2 ret(*this);
		ret *= v;
		return ret;
	}

	TemplateVec2 operator*(const T& t)
	{
		TemplateVec2 ret(*this);
		ret *= t;
		return ret;
	}

	TemplateVec2 operator/(const TemplateVec2& v)
	{
		TemplateVec2 ret(*this);
		ret /= v;
		return ret;
	}

	TemplateVec2 operator/(const T& t)
	{
		TemplateVec2 ret(*this);
		ret /= t;
		return ret;
	}

	TemplateVec2& operator=(const TemplateVec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	TemplateVec2& operator=(const T& v)
	{
		x = v;
		y = v;
		return *this;
	}

	TemplateVec2& operator+=(const TemplateVec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	TemplateVec2& operator+=(const T& v)
	{
		x += v;
		y += v;
		return *this;
	}

	TemplateVec2& operator-=(const TemplateVec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	TemplateVec2& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		return *this;
	}

	TemplateVec2& operator*=(const TemplateVec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	TemplateVec2& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		return *this;
	}

	TemplateVec2& operator/=(const TemplateVec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	TemplateVec2& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		return *this;
	}

	bool operator==(const TemplateVec2 &v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const TemplateVec2 &v) const
	{
		return !(*this == v);
	}

};
typedef TemplateVec2<int>Vec2_i;
typedef TemplateVec2<float>Vec2;
typedef TemplateVec2<double>Vec2_d;


template <typename T>
/*! @class Vec3
*   @brief 3次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さを測る機能があります
*/
class TemplateVec3
{
public:
	T x, y, z;
	TemplateVec3() :
		x(0),
		y(0),
		z(0)
	{}
	TemplateVec3(const T x, const T y, const T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	TemplateVec3(const TemplateVec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	~TemplateVec3() {};
	/*!
	* @brief オフセット値を返します
	* @return Vec3
	*/
	TemplateVec3 OffSetCopy(const T x, const T y, const T z)
	{
		TemplateVec3 ret(*this);
		ret.x += x;
		ret.y += y;
		ret.z += z;
		return ret;
	}
	/*!
	* @brief 内積を返します
	* @return float
	*/
	float Dot(TemplateVec3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	/*!
	* @brief 外積を返します
	* @return Vec3
	*/
	TemplateVec3 Cross(TemplateVec3& v)
	{
		TemplateVec3 result;
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		return result;
	}
	/*!
	* @brief 長さを返します
	* @return float
	*/
	T Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec3
	*/
	TemplateVec3&  Normalize()
	{
		T tmp = Length();
		x /= tmp;
		y /= tmp;
		z /= tmp;
		return *this;
	}
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec3
	*/
	TemplateVec3  GetNormalize() const
	{
		TemplateVec3 result = *this;
		result.Normalize();
		return result;
	}

	TemplateVec3&	operator=(const  TemplateVec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	TemplateVec3&	operator=(const  T& v)
	{
		x = v;
		y = v;
		z = v;
		return *this;
	}

	TemplateVec3 operator+() const
	{
		return TemplateVec3(*this);
	}

	TemplateVec3 operator-() const
	{
		return TemplateVec3(-this->x, -this->y, -this->z);
	}

	TemplateVec3 operator+(const TemplateVec3& v)
	{
		TemplateVec3 ret(*this);
		ret += v;
		return ret;
	}

	 TemplateVec3 operator+(const T& t)
	{
		 TemplateVec3 ret(*this);
		 ret += t;
		 return ret;
	}

	 TemplateVec3 operator-(const TemplateVec3& v)
	{
		 TemplateVec3 ret(*this);
		 ret -= v;
		return ret;
	}

	TemplateVec3 operator-(const T& t)
	{
		TemplateVec3 ret(*this);
		ret -= t;
		return ret;
	}

	TemplateVec3 operator*(const TemplateVec3& v)
	{
		TemplateVec3 ret(*this);
		ret *= v;
		return ret;
	}

	TemplateVec3 operator*(const T& t)
	{
		TemplateVec3 ret(*this);
		ret *= t;
		return ret;
	}

	TemplateVec3 operator/(const TemplateVec3& v)
	{
		TemplateVec3 ret(*this);
		ret /= v;
		return ret;
	}

	 TemplateVec3 operator/(const T& t)
	{
		 TemplateVec3 ret(*this);
		 ret /= t;
		 return ret;
	}

	TemplateVec3& operator+=(const TemplateVec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	TemplateVec3& operator+=(const T& v)
	{
		x += v;
		y += v;
		z += v;
		return *this;
	}

	TemplateVec3& operator-=(const TemplateVec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	TemplateVec3& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}

	TemplateVec3& operator*=(const TemplateVec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	TemplateVec3& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	TemplateVec3& operator/=(const TemplateVec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	TemplateVec3& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	bool	operator==(const TemplateVec3 &v) const
	{
		return (x == v.x && y == v.y && v.z == z);
	}
	bool	operator!=(const TemplateVec3 &v) const
	{
		return !(*this == v);
	}
	
};
typedef TemplateVec3<int>Vec3_i;
typedef TemplateVec3<float>Vec3;
typedef TemplateVec3<double>Vec3_d;

/*! @class Float4
*   @brief 4つのfloatの値を扱います
*   @detail DirectX11では、色の指定などに4つのfoat指定するので主にそれに使います
*/
class Float4
{
public:
	float r, g, b, a;
	Float4():
		r(0),
		g(0),
		b(0),
		a(0)
	{}
	Float4(float x, float y, float z, float w)
	{
		this->r = x;
		this->g = y;
		this->b = z;
		this->a = w;
	}
	Float4(const Float4& f4)
	{
		r = f4.r;
		g = f4.g;
		b = f4.b;
		a = f4.a;
	}
	~Float4() {};
	Float4&	operator=(const Float4& v)
	{
		r = v.r;
		g = v.g;
		b = v.b;
		a = v.a;
		return *this;
	}
	Float4& operator+=(const Float4& v)
	{
		r += v.r;
		g += v.g;
		b += v.b;
		a += v.a;
		return *this;
	}
	Float4& operator-=(const Float4& v)
	{
		r -= v.r;
		g -= v.g;
		b -= v.b;
		a -= v.a;
		return *this;
	}
	Float4& operator*=(const Float4& v)
	{
		r *= v.r;
		g *= v.g;
		b *= v.b;
		a *= v.a;
		return *this;
	}
	Float4& operator/=(const Float4& v)
	{
		r /= v.r;
		g /= v.g;
		b /= v.b;
		a /= v.a;
		return *this;
	}
	bool	operator==(const Float4 &v) const
	{
		return (r == v.r && g == v.g && v.b == b && a == v.a);
	}
	bool	operator!=(const Float4 &v) const
	{
		return !(*this == v);
	}

};