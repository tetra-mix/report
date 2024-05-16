#include "Timer.h"

Timer::Timer()
{
	reset();
}

Timer::~Timer()
{
	
}

void Timer::reset()
{
	start = GetTickCount();
}

double Timer::get()
{
	return (GetTickCount() - start) / 1000.0;
}