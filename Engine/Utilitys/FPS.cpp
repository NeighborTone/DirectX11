#include "FPS.h"
#include "../Engine.h"

LARGE_INTEGER FPS::GetCounter() const
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter;
}

LARGE_INTEGER FPS::GetCountFrequency() const
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency;
}

FPS::FPS():
	frameCount(0),
	frameRate(0),
	time(0),
	deltaTime(0),
	second(0)
{
	Engine::COMInitialize();

	preCount = GetCounter();
	frequency = GetCountFrequency();
}

FPS::~FPS()
{

}

void FPS::UpDate()
{
	LARGE_INTEGER count = GetCounter();
	deltaTime = static_cast<float>(count.QuadPart - preCount.QuadPart) / frequency.QuadPart;
	preCount = GetCounter();

	time += deltaTime;

	frameCount++;
	second += deltaTime;
	if (second >= 1.0f)
	{
		frameRate = frameCount;
		frameCount = 0;
		second -= 1.0f;
	}

}

float FPS::GetTime() const
{
	return time;
}

float FPS::GetDeltaTime() const
{
	return deltaTime;
}

int FPS::GetFrameRate() const
{
	return frameRate;
}
