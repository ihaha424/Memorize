// Timer.h
#pragma once

#include "framework.h"

class Timer {
	static double _precision;
	static double _timeScale;
public:
	/**
	 * @brief `QueryPerformanceFrequency`를 사용하여 high-resolution 타이머를
	 * 세팅하고, 정밀도를 측정합니다.
	 */
	static void InitTimer();
	/**
	 * @brief `QueryPerformanceCounter`를 사용하여 현재시간을 카운트하고,
	 * 정밀도를 사용하여 시간을 double 값으로 반환합니다.
	 * @return 현재 시간의 double 값
	 */
	static double GetTick();
	/**
	 * @brief 타임 스케일을 설정합니다. 값이 높아질수록 델타 타임이 커집니다.
	 * @param timeScale
	 */
	static void SetTimeScale(double timeScale);
	/**
	 * @brief 타임 스케일을 반환합니다.
	 * @return 타임 스케일. 기본값은 1.0.
	 */
	static double GetTimeScale();
};
