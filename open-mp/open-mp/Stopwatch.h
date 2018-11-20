#pragma once

#include <Windows.h>

class Stopwatch
{
private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER startPoint, endPoint;
	double elapsedMillis;
public:
	Stopwatch();
	~Stopwatch();

	void start();
	void stop();
	double elapsedMilliseconds();
};

