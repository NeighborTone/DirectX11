#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
class Easing
{
private:
	float	cnt;
	float	duration;
	bool	easeEnd;
public:
	//コンストラクタ
	Easing() : cnt(0), easeEnd(false),
		linear(&duration),
		back(&duration),
		bounce(&duration),
		circ(&duration),
		cubic(&duration),
		elastic(&duration),
		expo(&duration),
		quad(&duration),
		quart(&duration),
		quint(&duration),
		sine(&duration) 
	{}

	//イージング用カウンタ
	float Time(float dur)
	{
		duration = dur;
		if (cnt <= duration)
		{
			cnt += 0.1f;
			easeEnd = false;
		}
		else
		{
			easeEnd = true;
		}
		return cnt;
	}

	//イージングが終了したらtrueが返る
	bool IsEnd()
	{
		return easeEnd;
	}

	//t = 時間 d = 始点 c = 終点-始点 d = 経過時間
	class Linear
	{
		float* duration;
	public:
		//コンストラクタ
		Linear(float* dur) : duration(dur) {}

		float None(float t, float b, float c)
		{
			return c * t / *duration + b;
		}
		float In(float t, float b, float c)
		{
			return c * t / *duration + b;
		}
		float Out(float t, float b, float c)
		{
			return c * t / *duration + b;
		}
		float InOut(float t, float b, float c)
		{
			return c * t / *duration + b;
		}
	}linear;

	class Back
	{
		float* duration;
	public:
		//コンストラクタ
		Back(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			float s = 1.70158f;
			float postFix = t /= *duration;
			return c * (postFix)*t*((s + 1)*t - s) + b;
		}
		float Out(float t, float b, float c)
		{

			float s = 1.70158f;

			return c * ((t = t / *duration - 1)*t*((s + 1)*t + s) + 1) + b;
		}
		float InOut(float t, float b, float c)
		{
			float s = 1.70158f;
			if ((t /= *duration / 2) < 1) return c / 2 * (t*t*(((s *= (1.525f)) + 1)*t - s)) + b;
			float postFix = t -= 2;
			return c / 2 * ((postFix)*t*(((s *= (1.525f)) + 1)*t + s) + 2) + b;
		}
	}back;


	class Bounce
	{
		float* duration;
	public:
		//コンストラクタ
		Bounce(float* dur) : duration(dur) {}

		float Out(float t, float b, float c)
		{

			if ((t /= *duration) < (1 / 2.75f))
			{
				return c * (7.5625f*t*t) + b;
			}
			else if (t < (2 / 2.75f))
			{
				float postFix = t -= (1.5f / 2.75f);
				return c * (7.5625f*(postFix)*t + .75f) + b;
			}
			else if (t < (2.5 / 2.75))
			{
				float postFix = t -= (2.25f / 2.75f);
				return c * (7.5625f*(postFix)*t + .9375f) + b;
			}
			else
			{
				float postFix = t -= (2.625f / 2.75f);
				return c * (7.5625f*(postFix)*t + .984375f) + b;
			}
		}
		float In(float t, float b, float c)
		{
			return c - Out(*duration - t, 0, c) + b;
		}
		float InOut(float t, float b, float c)
		{
			if (t < *duration / 2) return In(t * 2, 0, c) * .5f + b;
			else return Out(t * 2 - *duration, 0, c) * .5f + c * .5f + b;
		}
	}bounce;

	class Circ
	{
		float* duration;
	public:
		//コンストラクタ
		Circ(float* dur) : duration(dur) {}

		float In(float t, float b, float c)	//バグあり
		{
			return -c * static_cast<float>((sqrt(1 - (t /= *duration)*t) - 1)) + b;
		}
		float Out(float t, float b, float c)
		{
			return c * static_cast<float>(sqrt(1 - (t = t / *duration - 1)*t)) + b;
		}
		float InOut(float t, float b, float c)
		{
			if ((t /= *duration / 2) < 1) return -c / 2 * static_cast<float>((sqrt(1 - t * t) - 1)) + b;
			return c / 2 * static_cast<float>((sqrt(1 - t * (t -= 2)) + 1)) + b;
		}
	}circ;

	class Cubic
	{
		float* duration;
	public:
		//コンストラクタ
		Cubic(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return c * (t /= *duration)*t*t + b;
		}
		float Out(float t, float b, float c)
		{
			return c * ((t = t / *duration - 1)*t*t + 1) + b;
		}
		float InOut(float t, float b, float c)
		{
			if ((t /= *duration / 2) < 1) return c / 2 * t*t*t + b;
			return c / 2 * ((t -= 2)*t*t + 2) + b;
		}
	}cubic;

	class Elastic
	{
		float* duration;
	public:
		//コンストラクタ
		Elastic(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			if (t == 0) return b;  if ((t /= *duration) == 1) return b + c;
			float p = *duration*.3f;
			float a = c;
			float s = p / 4;
			float postFix = static_cast<float>(a*pow(2, 10 * (t -= 1)));
			return -static_cast<float>(postFix * sin((t*(*duration) - s)*(2 * (M_PI)) / p)) + b;
		}

		float Out(float t, float b, float c)
		{
			if (t == 0) return b;  if ((t /= *duration) == 1) return b + c;
			float p = *duration*.3f;
			float a = c;
			float s = p / 4;
			return (static_cast<float>(a*pow(2, -10 * t) * sin((t*(*duration) - s)*(2 * static_cast<float>(M_PI)) / p) + c + b));
		}

		float InOut(float t, float b, float c)
		{
			if (t == 0) return b;  if ((t /= *duration / 2) == 2) return b + c;
			float p = *duration*(.3f*1.5f);
			float a = c;
			float s = p / 4;

			if (t < 1) {
				float postFix = static_cast<float>(a*pow(2, 10 * (t -= 1)));
				return -.5f*static_cast<float>((postFix* sin((t*(*duration) - s)*(2 * (M_PI)) / p))) + b;
			}
			float postFix = static_cast<float>(a*pow(2, -10 * (t -= 1)));
			return static_cast<float>(postFix * sin((t*(*duration) - s)*(2 * static_cast<float>(M_PI)) / p)*.5f + c + b);
		}
	}elastic;

	class Expo
	{
		float* duration;
	public:
		//コンストラクタ
		Expo(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return (t == 0) ? b : c * static_cast<float>(pow(2, 10 * (t / *duration - 1)) + b);
		}
		float Out(float t, float b, float c)
		{
			return (t == *duration) ? b + c : c * (static_cast<float>(-pow(2, -10 * t / *duration) + 1)) + b;
		}
		float InOut(float t, float b, float c)
		{
			if (t == 0) return b;
			if (t == *duration) return b + c;
			if ((t /= *duration / 2) < 1) return c / 2 * static_cast<float>(pow(2, 10 * (t - 1))) + b;
			return c / 2 * (static_cast<float>(-pow(2, -10 * --t) + 2)) + b;
		}
	}expo;

	class Quad
	{
		float* duration;
	public:
		//コンストラクタ
		Quad(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return c * (t /= *duration)*t + b;
		}
		float Out(float t, float b, float c)
		{
			return -c * (t /= *duration)*(t - 2) + b;
		}
		float InOut(float t, float b, float c)
		{
			if ((t /= *duration / 2) < 1) return ((c / 2)*(t*t)) + b;
			return -c / 2 * (((t - 2)*(--t)) - 1) + b;
		}
	}quad;

	class Quart
	{
		float* duration;
	public:
		//コンストラクタ
		Quart(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return c * (t /= *duration)*t*t*t + b;
		}
		float Out(float t, float b, float c)
		{
			return -c * ((t = t / *duration - 1)*t*t*t - 1) + b;
		}
		float InOut(float t, float b, float c)
		{
			if ((t /= *duration / 2) < 1) return c / 2 * t*t*t*t + b;
			return -c / 2 * ((t -= 2)*t*t*t - 2) + b;
		}
	}quart;

	class Quint
	{
		float* duration;
	public:
		//コンストラクタ
		Quint(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return c * (t /= *duration)*t*t*t*t + b;
		}
		float Out(float t, float b, float c)
		{
			return c * ((t = t / *duration - 1)*t*t*t*t + 1) + b;
		}
		float InOut(float t, float b, float c)
		{
			if ((t /= *duration / 2) < 1) return c / 2 * t*t*t*t*t + b;
			return c / 2 * ((t -= 2)*t*t*t*t + 2) + b;
		}
	}quint;

	class Sine
	{
		float* duration;
	public:
		//コンストラクタ
		Sine(float* dur) : duration(dur) {}

		float In(float t, float b, float c)
		{
			return -c * static_cast<float>(cos(t / *duration * (M_PI / 2))) + c + b;
		}
		float Out(float t, float b, float c)
		{
			return c * static_cast<float>(sin(t / *duration * (M_PI / 2))) + b;
		}

		float InOut(float t, float b, float c)
		{
			return -c / 2 * static_cast<float>((cos(M_PI*t / *duration) - 1)) + b;
		}
	}sine;

};