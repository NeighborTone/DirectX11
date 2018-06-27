#pragma once

template <typename T>
/*! @class Counter
*   @brief �J�E���^�[�ł�.����l�ɒB�����牺���l�A�����l�𒴂��������l�ɂȂ�܂�
*   @detail �R���X�g���N�^�ŏ����l�A���Z�l�A�����l�A����l��ݒ肵�Ă�������
*   @detail int(Counter), float(CounterF), double(CounterD)�^�����ꂼ�ꂠ��܂�
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
	* @brief �J�E���^�[�����Z�l�������܂�
	* @return ���݂̃J�E���^�[�̒l
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
	* @brief �J�E���^�[�����Z�l�������܂�
	* @return ���݂̃J�E���^�[�̒l
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
	* @brief ���݂̃J�E���^�[�̒l��Ԃ��܂�
	* @return ���݂̃J�E���^�[�̒l
	*/
	T GetCurrentCount()
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

typedef TemplateCounter<int>Counter;
typedef TemplateCounter<float>Counter_f;
typedef TemplateCounter<double>Counter_d;