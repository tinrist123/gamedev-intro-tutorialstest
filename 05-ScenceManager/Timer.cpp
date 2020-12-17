#include "Timer.h"

Timer::Timer(ULONGLONG limitedTime)
{
	startTime = 0;
	this->limitedTime = limitedTime;
}

void Timer::Start()
{
	startTime = GetTickCount64();
	this->isStarted = true;
}

void Timer::Stop()
{
	startTime = 0;
}

bool Timer::IsTimeUp()
{
	return (startTime && GetTickCount64() - startTime > limitedTime);
}

bool Timer::ResetTime()
{
	return startTime == 0;
}

ULONGLONG Timer::GetElapsedTime()
{
	return GetTickCount64() - startTime;
}

Timer::~Timer()
{
}
