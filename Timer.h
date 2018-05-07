#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
public:
	static void Update();

	static float GetDeltaTime();
private:
	static __int64 prevFrame;
	static __int64 currentFrame;
	static __int64 freq;
	static float deltaTime;
};

#endif