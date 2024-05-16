#pragma once
#include <Windows.h>

class Timer
{
	DWORD start;
public:
	Timer();
	virtual ~Timer();

	void reset();
	double get();
};