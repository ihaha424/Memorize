#pragma once

#include "common.h"

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

	/**
	 * @brief 타미어: 특정 시간 마다 매핑된 함수를 실행시켜준다. 루프가 아니면 1면 루프면 계속
	 * 또한 조건식(bool)을 넣어서 Loop를 종료 시킬 수도 있다.
	 * 조건식이 참이면 종료, 기본 값 false
	 * @param deltaTime 
	 * @param _condition 조건식으로 참이면 종료한다.
	 */
	void Update(const float& deltaTime, bool _condition = false)
	{
		if (!Finish && !_condition)
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
		else
			CurTime = 0.f;
	}

	void SetDelayTime(float _DelayTime) { DelayTime = _DelayTime; }
	void SetLoop(bool _Loop) { Loop = _Loop; }
	void SetFinish(bool _Finish) { Finish = _Finish; }
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

