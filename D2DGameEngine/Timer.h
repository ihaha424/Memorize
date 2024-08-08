// Timer.h
#pragma once

#include "framework.h"

class Timer {
	static double precision;
	static double prevTime;
	static double currTime;
	static double frameTime;
	static double timeScale;
public:
	/**
	 * @brief `QueryPerformanceFrequency`를 사용하여 high-resolution 타이머를
	 * 세팅하고, 정밀도를 측정합니다.
	 */
	static void InitTimer();

	/**
	 * @brief `QueryPerformanceCounter`를 사용하여 현재시간을 카운트하고,
	 * 정밀도를 사용하여 frameTime을 업데이트 합니다.
	 */
	static void UpdateTime();

	/**
	 * @brief
	 * @return 현재 시간의 RealTime 값을 리턴 합니다.
	 */
	static double GetRealTick();

	/**
	 * @brief 
	 * @return 현재 시간의 frameTime 값을 리턴 합니다.
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
