#pragma once
#include "Vector.h"
#include <Windows.h>
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
	T GetCurrentTime()
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
typedef _Counter<float>CounterF;
typedef _Counter<double>CounterD;