#pragma once

#include "EasingFunction.h"

enum StepAnimation
{
	StepOnceForward,
	StepOnceBack,
	StepOncePingPong,
	StepLoopForward,
	StepLoopBack,
	StepLoopPingPong,

	StepAnimationEnd
};

template<typename T>
class DotTween
{
public:
	DotTween() {}
	DotTween(T* _data, EasingEffect _easingEffect, StepAnimation _stepAnimation = StepOnceForward)
		:data(_data), function(EasingFunction[_easingEffect]), type(_stepAnimation)
	{}
	~DotTween() {};

	void	SetData(T* _data)								{ data = _data; }
	void	SetStartPoint(T _startPoint)					{ startPoint = _startPoint; }
	void	SetEndPoint(T _endPoint)						{ endPoint = _endPoint; }
	void	SetDuration(float	_duration)					{ duration = _duration; }
	void	SetEasingEffect(EasingEffect	_easingEffect)	{ function = EasingFunction[_easingEffect]; }
	void	SetStepAnimation(StepAnimation	_stepAnimation)	{ type = _stepAnimation; }

	void	ResetTime() { curTime = 0.f; }
	bool	GetIsFinish() { return (curTime > duration); }

	void	Update(const float& _deltaTime)
	{
		(this->*StepAnimationfunction[type])(_deltaTime);
	}

private:
	T*							data;			//	0 ~ 1(startPoint ~ endPoint)
	T							startPoint;
	T							endPoint;
	float						duration;		// N초 동안
	float						curTime = 0.f;
	std::function<float(float)>	function;
	StepAnimation				type;

private:
	void OnceForward(const float& _DeltaTime)
	{
		if (curTime > duration)
			return;
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		CurStepTime = curTime / duration;
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}
	void OnceBack(const float& _DeltaTime)
	{
		if (curTime > duration)
			return;
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		CurStepTime = 1.f - (curTime / duration);
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}
	void OncePingPong(const float& _DeltaTime)
	{
		if (curTime > duration)
			return;
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		CurStepTime = (curTime < (duration / 2))
			? curTime / (duration / 2)
			: 1.f - ((curTime - duration / 2) / (duration / 2));
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}
	void LoopForward(const float& _DeltaTime)
	{
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		if (curTime > duration)
			curTime -= duration;
		CurStepTime = curTime / duration;
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}
	void LoopBack(const float& _DeltaTime)
	{
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		if (curTime > duration)
			curTime -= duration;
		CurStepTime = 1.f - (curTime / duration);
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}
	void LoopPingPong(const float& _DeltaTime)
	{
		float CurStepTime;
		T CurStep;

		curTime += _DeltaTime;
		if (curTime > duration)
			curTime -= duration;
		CurStepTime = (curTime < (duration / 2))
			? curTime / (duration / 2)
			: 1.f - ((curTime - duration / 2) / (duration / 2));
		CurStep = endPoint - startPoint;

		*data = startPoint + CurStep * function(CurStepTime);
	}

	static void (DotTween::* StepAnimationfunction[StepAnimation::StepAnimationEnd])(const float&);
};

// 정적 멤버 함수 포인터 배열 초기화
template<typename T>
void (DotTween<T>::* DotTween<T>::StepAnimationfunction[StepAnimation::StepAnimationEnd])(const float&) =
{
	&DotTween<T>::OnceForward,
	&DotTween<T>::OnceBack,
	&DotTween<T>::OncePingPong,
	&DotTween<T>::LoopForward,
	&DotTween<T>::LoopBack,
	&DotTween<T>::LoopPingPong
};

