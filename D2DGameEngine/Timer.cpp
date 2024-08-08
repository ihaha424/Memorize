// Timer.cc
#include "Timer.h"

double Timer::timeScale = 1.0;
double Timer::precision = 0.0;
double Timer::prevTime = 0.0;
double Timer::currTime = 0.0;
double Timer::frameTime = 0.0;

void Timer::InitTimer() {
	LARGE_INTEGER freq;
	if (!QueryPerformanceFrequency(&freq)) {
		// TODO: Need a logger!
		throw std::runtime_error("Timer: Performance counter is not supported!");
	}
	precision = 1.0 / static_cast<double>(freq.QuadPart);
}

void Timer::UpdateTime()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	currTime = static_cast<double>(counter.QuadPart) * precision;
	frameTime = currTime - prevTime;	// 델타 타임
	prevTime = currTime;
}

double Timer::GetRealTick()
{
	return frameTime;
}

double Timer::GetTick() {
	return frameTime * timeScale;
}

void Timer::SetTimeScale(double timeScale) {
	timeScale = timeScale;
}

double Timer::GetTimeScale()
{
	return timeScale;
}
