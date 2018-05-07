#include "Timer.h"

__int64 Timer::prevFrame = 0;
__int64 Timer::currentFrame = 0;
__int64 Timer::freq = 0;
float Timer::deltaTime = 0;

void Timer::Update()
{
	deltaTime = 0.0f;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&currentFrame);
	int deltaTicks = (int)(currentFrame - prevFrame);
	deltaTime = ((float)deltaTicks/(float)freq);
	prevFrame = currentFrame;
}

float Timer::GetDeltaTime()
{
	float dt = deltaTime;
	if(dt <= 0.0f || dt > 1.0f)
	{
		dt = 0.025f;
	}

	return dt;
}