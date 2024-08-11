#pragma once

#include "D2DGameEngine/framework.h"

// T의 타입을 std::function<ReturnValue(ARGS...)>으로 제한하는 것.
template <typename ReturnValue, typename... ARGS>
class CoolTime
{
public:
	// T는 std::function<ReturnValue(ARGS...)>로 변환 가능해야 함.
	template <typename T, typename = typename std::enable_if<std::is_convertible<T, std::function<ReturnValue(ARGS...)>>::value>::type>
	CoolTime(T _Function, float _DelayTime, bool _Loop = false)
		: Function(_Function), DelayTime(_DelayTime), Loop(_Loop)
	{}

	~CoolTime() {}

	void Update(const float& deltaTime)
	{
		if (!Finish)
		{
			CurTime += deltaTime;
			if (DelayTime <= CurTime)
			{
				Function();
				if (!Loop)
					Finish = true;
				else
					CurTime -= DelayTime;
			}
		}
	}

	void SetDelayTime(float _DelayTime) { DelayTime = _DelayTime; }
	void SetLoop(bool _Loop) { Loop = _Loop; }
	bool GetFinish() const { return Finish; }

private:
	std::function<ReturnValue(ARGS...)> Function;
	float DelayTime;
	float CurTime = 0.f;
	bool Loop;
	bool Finish = false;
};

//	Ex>
//	Engine::Timer<std::function<void()>, void> timer(MyFunction, 1.0f, true);
//	timer.Update(deltaTime);

