#include "pch.h"

Stopwatch::Stopwatch()
{
	QueryPerformanceFrequency(&this->frequency);
	this->elapsedMillis = 0.0;
}


Stopwatch::~Stopwatch()
{
	this->elapsedMillis = 0.0;
}

void Stopwatch::start()
{
	QueryPerformanceCounter(&this->startPoint);
}

void Stopwatch::stop()
{
	QueryPerformanceCounter(&this->endPoint);
	this->elapsedMillis = (this->endPoint.QuadPart - this->startPoint.QuadPart) * 1000.0 / frequency.QuadPart;
}

double Stopwatch::elapsedMilliseconds()
{
	return this->elapsedMillis;
}
