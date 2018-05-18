#pragma once
#include <Windows.h>
#include <math.h>

template <typename T>
/*! @class Vec2
*   @brief 2�����x�N�g���������܂�
*   @detail ���ρA�O�ρA���K���A�����𑪂�@�\������܂�
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
	* @brief ���ς�Ԃ��܂�
	* @return ����
	*/
	T Dot(_Vec2& v)
	{
		return x * v.x + y * v.y;
	}

	/*!
	* @brief �O�ς�Ԃ��܂�
	* @return �O��
	*/
	T Cross(_Vec2& v)
	{
		return x * v.y - v.x * y;
	}

	/*!
	* @brief ������Ԃ��܂�
	* @return ����
	*/
	T Length()
	{
		return sqrtf(x * x + y * y);
	}
	/*!
	* @brief �������g�𐳋K�������l��Ԃ��܂�
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
	* @brief �������g�𐳋K�������l�̃R�s�[��Ԃ��܂��B���g�̒l�͕ς��܂���
	* @return Vec2
	*/
	_Vec2  GetNormalize() const
	{
		_Vec2 result = *this;
		result.Normalize();
		return result;
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
	_Vec2& operator-=(const _Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	_Vec2& operator*=(const _Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	_Vec2& operator/=(const _Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	bool	operator==(const _Vec2 &v) const
	{
		return x == v.x && y == v.y;
	}
	bool	operator!=(const _Vec2 &v) const
	{
		return !(*this == v);
	}

};
typedef _Vec2<int>Vec2_i;
typedef _Vec2<float>Vec2;
typedef _Vec2<double>Vec2_d;


template <typename T>
/*! @class Vec3
*   @brief 3�����x�N�g���������܂�
*   @detail ���ρA�O�ρA���K���A�����𑪂�@�\������܂�
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
	* @brief ���ς�Ԃ��܂�
	* @return float
	*/
	float Dot(_Vec3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	/*!
	* @brief �O�ς�Ԃ��܂�
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
	* @brief ������Ԃ��܂�
	* @return float
	*/
	T Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	/*!
	* @brief �������g�𐳋K�������l��Ԃ��܂�
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
	* @brief �������g�𐳋K�������l�̃R�s�[��Ԃ��܂��B���g�̒l�͕ς��܂���
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
	_Vec3& operator+=(const _Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	_Vec3& operator-=(const _Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	_Vec3& operator*=(const _Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	_Vec3& operator/=(const _Vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
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
*   @brief 4��float�̒l�������܂�
*   @detail DirectX11�ł́A�F�̎w��Ȃǂ�4��foat�w�肷��̂Ŏ�ɂ���Ɏg���܂�
*/
class Float4
{
public:
	float x, y, z, w;
	Float4():
		x(0),
		y(0),
		z(0),
		w(0)
	{}
	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Float4(const Float4& f4)
	{
		x = f4.x;
		y = f4.y;
		z = f4.z;
		w = f4.w;
	}
	~Float4() {};
	Float4&	operator=(const Float4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	Float4& operator+=(const Float4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	Float4& operator-=(const Float4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	Float4& operator*=(const Float4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	Float4& operator/=(const Float4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	bool	operator==(const Float4 &v) const
	{
		return (x == v.x && y == v.y && v.z == z && w == v.w);
	}
	bool	operator!=(const Float4 &v) const
	{
		return !(*this == v);
	}

};

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
struct Vertex
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
};

template <typename T>
/*! @class Counter
*   @brief �J�E���^�[�ł�.����l�ɒB�����牺���l�A�����l�𒴂��������l�ɂȂ�܂�
*   @detail �R���X�g���N�^�ŏ����l�A���Z�l�A�����l�A����l��ݒ肵�Ă�������
*   @detail int(Counter), float(CounterF), double(CounterD)�^�����ꂼ�ꂠ��܂�
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
	* @brief �J�E���^�[�����Z�l�������܂�
	* @return ���݂̃J�E���^�[�̒l
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
	* @brief �J�E���^�[�����Z�l�������܂�
	* @return ���݂̃J�E���^�[�̒l
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
	* @brief ���݂̃J�E���^�[�̒l��Ԃ��܂�
	* @return ���݂̃J�E���^�[�̒l
	*/
	T GetCurrentTime()
	{
		return now;
	}

	/*!
	* @brief ���݂̃J�E���^�[�̒l��1�x�ł�����l�𒴂�������Ԃ��܂�
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
	* @brief ���݂̃J�E���^�[�̒l��1�x�ł������l�𒴂�������Ԃ��܂�
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