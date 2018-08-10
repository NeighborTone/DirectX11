#pragma once
#pragma warning (disable : 4458)	//this���g�����ߏ���
template <typename T>
/*! @class Counter
*   @brief �J�E���^�[�ł�.����l�ɒB�����牺���l�A�����l�𒴂��������l�ɂȂ�܂�
*   @note �R���X�g���N�^�ŏ����l�A���Z�l�A�����l�A����l��ݒ肵�Ă�������
*   ����l�≺���l��ݒ肵�Ă��Ȃ��ꍇ�A���̌^�̍ő�l�A�ŏ��l���ݒ肳��܂�
*   �����w�肵�Ȃ��ꍇ�̉��Z�l��1�ɂȂ�܂�
*    int(Counter), float(CounterF), double(CounterD)�^�����ꂼ�ꂠ��܂�
*/
class TemplateCounter
{
#undef max
#undef min
private:
	T now;
	T add;
	T min;
	T max;
	bool isMax;
	bool isMin;
public:
	
	TemplateCounter() :
		isMax(false),
		isMin(false),
		now(0),
		add(1),
		min(std::numeric_limits<T>::min()),
		max(std::numeric_limits<T>::max())
	{};
	//!���Z�l�̂ݐݒ�
	explicit TemplateCounter(T add) :
		isMax(false),
		isMin(false),
		now(0),
		add(add),
		min(std::numeric_limits<T>::min()),
		max(std::numeric_limits<T>::max())
	{};
	//!�����l�Ə���l�ݒ�
	TemplateCounter(T min, T max) :
		isMax(false),
		isMin(false),
		now(0),
		add(1),
		min(min),
		max(max)
	{};
	//!���ׂẴp�����[�^�ݒ�
	TemplateCounter(T now, T add, T min, T max) :
		isMax(false),
		isMin(false)
	{
		this->now = now;
		this->add = add;
		this->min = min;
		this->max = max;
	}
	//!�J�E���^�[�����������܂�
	void SetCounter(T now, T add, T min, T max) 
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
		if (now >= max)
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
		if (now <= min)
		{
			now = max;
		}
		else
		{
			now -= add;
		}
		return now;
	}
	//!�I�����Ԃ��܂ŉ��Z�l�����₵�܂��B����ȏ�ɂȂ������~���܂�
	void Add()
	{
		if (now >= max)
		{
			return;
		}
		now += add;
	}
	//!�I�����Ԃ��܂ŉ��Z�l�������܂��B�����ȉ��ɂȂ������~���܂�
	void Sub()
	{
		if (now <= min)
		{
			return;
		}
		now -= add;
	}
	//!�o�ߎ��Ԃ�0�ɂ��A�t���O���ăZ�b�g���܂�
	void Reset()
	{
		isMax = false;
		isMin = false;
		now = 0;
	}

	//!���݂̃J�E���^�[�̒l���~���b�ŕԂ��܂�
	T GetMilliSecond(T frameRate)
	{
		return (now / frameRate) * 1000;
	}

	//!�I�����Ԃ�ݒ肵�܂�
	void SetEndTime(T max, T min)
	{
		this->max = max;
		this->min = min;
	}
	//!�I�����Ԃ�ݒ肵�܂�
	void SetEndTime(T endTime)
	{
		this->max = endTime;
		this->min = -endTime;
	}
	//!�I�����Ԃ��^�̍ő�l�ɂ��܂�
	void SetInfinity()
	{
		max = std::numeric_limits<T>::max();
		min = std::numeric_limits<T>::min();
	}

	//�c�莞��(�I������-�o�ߎ���)��Ԃ�
	T GetRemainingTime() const
	{
		return max - now;
	}
	/*!
	* @brief ���݂̃J�E���^�[�̒l��Ԃ��܂�
	* @return ���݂̃J�E���^�[�̒l
	*/
	const T& GetCurrentCount() const
	{
		return now;
	}

	/*!
	* @brief ���݂̃J�E���^�[�̒l��1�x�ł�����l�ȏ�ɓ��B�������Ԃ��܂�
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
	* @brief ���݂̃J�E���^�[�̒l��1�x�ł������l�ȉ��ɓ��B�������Ԃ��܂�
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
typedef TemplateCounter<long>Counter_l;
typedef TemplateCounter<long long>Counter_ll;
typedef TemplateCounter<float>Counter_f;
typedef TemplateCounter<double>Counter_d;