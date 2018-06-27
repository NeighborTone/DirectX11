#pragma once

template <typename T>
/*! @class Counter
*   @brief カウンターです.上限値に達したら下限値、下限値を超えたら上限値になります
*   @note コンストラクタで初期値、加算値、下限値、上限値を設定してください
*   @note int(Counter), float(CounterF), double(CounterD)型がそれぞれあります
*/
class TemplateCounter
{
private:
	T now;
	T add;
	T min;
	T max;
	bool isMax;
	bool isMin;
public:
	TemplateCounter(T now, T add, T min, T max) :
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
	T operator++() 
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
	T operator--()
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

typedef TemplateCounter<int>Counter;
typedef TemplateCounter<float>Counter_f;
typedef TemplateCounter<double>Counter_d;