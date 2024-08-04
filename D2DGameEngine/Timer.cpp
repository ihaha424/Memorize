// Timer.cc
#include "Timer.h"

double Timer::_timeScale = 1.0;
double Timer::_precision = 0.0;

void Timer::InitTimer() {
	LARGE_INTEGER freq;
	if (!QueryPerformanceFrequency(&freq)) {
		// TODO: Need a logger!
		throw std::runtime_error("Timer: Performance counter is not supported!");
	}
	_precision = 1.0 / static_cast<double>(freq.QuadPart);
}

double Timer::GetTick() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return static_cast<double>(counter.QuadPart) * _precision;
}

void Timer::SetTimeScale(double timeScale) {
	_timeScale = timeScale;
}

double Timer::GetTimeScale()
{
	return _timeScale;
}
