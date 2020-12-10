#pragma once
#include "Windows.h"


class Timer
{
	ULONGLONG startTime, limitedTime;
public:
	Timer(ULONGLONG limitedTime);

	void Start();
	void Stop();
	bool IsTimeUp();
	bool ResetTime();
	ULONGLONG GetElapsedTime();
	~Timer();
};