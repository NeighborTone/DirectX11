#pragma once
#include <Windows.h>
#include <math.h>

template <typename T>
/*! @class Vec2
*   @brief 2次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さを測る機能があります
*/
class _Vec2
{
public:
	T x, y;
	_Vec2():
	x(0),
	y(0)
	{};
	_Vec2(T x, T y)
	{
		this->x = x;
		this->y = y;
	};
	_Vec2(const _Vec2& v)
	{
		x = v.x;
		y = v.y;
	}
	~_Vec2() {};

	/*!
	* @brief 内積を返します
	* @return 内積
	*/
	T Dot(_Vec2& v)
	{
		return x * v.x + y * v.y;
	}

	/*!
	* @brief 外積を返します
	* @return 外積
	*/
	T Cross(_Vec2& v)
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
	_Vec2&  Normalize()
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
	_Vec2  GetNormalize() const
	{
		_Vec2 result = *this;
		result.Normalize();
		return result;
	}

	_Vec2 operator+() const
	{
		return _Vec2(*this);
	}

	_Vec2 operator-() const
	{
		return _Vec2(-x, -y);
	}

	friend _Vec2 operator+(const _Vec2& t1, const _Vec2& t2)
	{
		return _Vec2(t1) += t2;
	}

	friend _Vec2 operator+(const _Vec2& t1, const T& t2)
	{
		return _Vec2(t1) += t2;
	}

	friend _Vec2 operator-(const _Vec2& t1, const _Vec2& t2)
	{
		return _Vec2(t1) -= t2;
	}

	friend _Vec2 operator-(const _Vec2& t1, const T& t2)
	{
		return _Vec2(t1) -= t2;
	}

	friend _Vec2 operator*(const _Vec2& t1, const _Vec2& t2)
	{
		return _Vec2(t1) *= t2;
	}

	friend _Vec2 operator*(const _Vec2& t1, const T& t2)
	{
		return _Vec2(t1) *= t2;
	}

	friend _Vec2 operator/(const _Vec2& t1, const _Vec2& t2)
	{
		return _Vec2(t1) /= t2;
	}

	friend _Vec2 operator/(const _Vec2& t1, const T& t2)
	{
		return _Vec2(t1) /= t2;
	}

	_Vec2& operator=(const _Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	_Vec2& operator+=(const _Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	_Vec2& operator+=(const T& v)
	{
		x += v;
		y += v;
		return *this;
	}

	_Vec2& operator-=(const _Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	_Vec2& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		return *this;
	}

	_Vec2& operator*=(const _Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	_Vec2& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		return *this;
	}

	_Vec2& operator/=(const _Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	_Vec2& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		return *this;
	}

	bool operator==(const _Vec2 &v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const _Vec2 &v) const
	{
		return !(*this == v);
	}

};
typedef _Vec2<int>Vec2_i;
typedef _Vec2<float>Vec2;
typedef _Vec2<double>Vec2_d;


template <typename T>
/*! @class Vec3
*   @brief 3次元ベクトルを扱います
*   @detail 内積、外積、正規化、長さを測る機能があります
*/
class _Vec3
{
public:
	T x, y, z;
	_Vec3() :
		x(0),
		y(0),
		z(0)
	{}
	_Vec3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	_Vec3(const _Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	~_Vec3() {};

	/*!
	* @brief 内積を返します
	* @return float
	*/
	float Dot(_Vec3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	/*!
	* @brief 外積を返します
	* @return Vec3
	*/
	_Vec3 Cross(_Vec3& v)
	{
		_Vec3 result;
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
	_Vec3&  Normalize()
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
	_Vec3  GetNormalize() const
	{
		_Vec3 result = *this;
		result.Normalize();
		return result;
	}

	_Vec3&	operator=(const  _Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	_Vec3&	operator=(const  T& v)
	{
		x = v;
		y = v;
		z = v;
		return *this;
	}

	_Vec3 operator+() const
	{
		return _Vec3(*this);
	}

	_Vec3 operator-() const
	{
		return _Vec3(-this->x, -this->y, -this->z);
	}

	friend _Vec3 operator+(const _Vec3& t1, const _Vec3& t2)
	{
		return _Vec3(t1) += t2;
	}

	friend _Vec3 operator+(const _Vec3& t1, const T& t2)
	{
		return _Vec3(t1) += t2;
	}

	friend _Vec3 operator-(const _Vec3& t1, const _Vec3& t2)
	{
		return _Vec3(t1) -= t2;
	}

	friend _Vec3 operator-(const _Vec3& t1, const T& t2)
	{
		return _Vec3(t1) -= t2;
	}

	friend _Vec3 operator*(const _Vec3& t1, const _Vec3& t2)
	{
		return _Vec3(t1) *= t2;
	}

	friend _Vec3 operator*(const _Vec3& t1, const T& t2)
	{
		return _Vec3(t1) *= t2;
	}

	friend _Vec3 operator/(const _Vec3& t1, const _Vec3& t2)
	{
		return _Vec3(t1) /= t2;
	}

	friend _Vec3 operator/(const _Vec3& t1, const T& t2)
	{
		return _Vec3(t1) /= t2;
	}

	_Vec3& operator+=(const _Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	_Vec3& operator+=(const T& v)
	{
		x += v;
		y += v;
		z += v;
		return *this;
	}

	_Vec3& operator-=(const _Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	_Vec3& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}

	_Vec3& operator*=(const _Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	_Vec3& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	_Vec3& operator/=(const _Vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	_Vec3& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	bool	operator==(const _Vec3 &v) const
	{
		return (x == v.x && y == v.y && v.z == z);
	}
	bool	operator!=(const _Vec3 &v) const
	{
		return !(*this == v);
	}

};
typedef _Vec3<int>Vec3_i;
typedef _Vec3<float>Vec3;
typedef _Vec3<double>Vec3_d;

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

/*! @class ErrorMessage
*   @brief エラーメッセージをメッセージボックスで出します	
*   @detail コンストラクタでチェック用変数と文字列を設定してください
*   @detail HRUSULT, int, bool型のいずれかでチェックできます
*/
class ErrorMessage
{
public:
	ErrorMessage(HRESULT hr, const char* text, const char* category)
	{
		if (FAILED(hr))
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	ErrorMessage(int isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	ErrorMessage(bool isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
};

/*! @class Vertex
*   @brief 頂点用の構造体です
*   @detail 位置情報、法線、UV値を格納できます
*/
struct Vertex
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;

	Vertex(Vec3 pos, Vec3 normal, Vec2 uv)
	{
		this->pos = pos;
		this->normal = normal;
		this->uv = uv;
	}
};

template <typename T>
/*! @class Counter
*   @brief カウンターです.上限値に達したら下限値、下限値を超えたら上限値になります
*   @detail コンストラクタで初期値、加算値、下限値、上限値を設定してください
*   @detail int(Counter), float(CounterF), double(CounterD)型がそれぞれあります
*/
class _Counter
{
private:
	T now;
	T add;
	T min;
	T max;
	bool isMax;
	bool isMin;
public:
	_Counter(T now, T add, T min, T max) :
		isMax(false),
		isMin(false)
	{
		this->now = now;
		this->add = add;
		this->min = min;
		this->max = max;
	}

	/*!
	* @brief カウンターを加算値分足します
	* @return 現在のカウンターの値
	*/
	void operator++() const
	{
		if (now > max)
		{
			now = min;
		}
		else
		{
			now += add;
		}
		return now;
	}

	/*!
	* @brief カウンターを加算値分引きます
	* @return 現在のカウンターの値
	*/
	void operator--() const
	{
		if (now < min)
		{
			now = max;
		}
		else
		{
			now -= add;
		}
		return now;
	}

	/*!
	* @brief 現在のカウンターの値を返します
	* @return 現在のカウンターの値
	*/
	T GetCurrentCount()
	{
		return now;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも上限値を超えたかを返します
	* @return bool
	*/
	bool IsMax()
	{
		if (now >= max)
		{
			isMax = true;
		}
		return isMax;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも下限値を超えたかを返します
	* @return bool
	*/
	bool IsMin()
	{
		if (now <= min)
		{
			isMin = true;
		}
		return isMin;
	}
};

typedef _Counter<int>Counter;
typedef _Counter<float>Counter_f;
typedef _Counter<double>Counter_d;