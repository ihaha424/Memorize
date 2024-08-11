#pragma once

#include "D2DGameEngine/framework.h"

// T�� Ÿ���� std::function<ReturnValue(ARGS...)>���� �����ϴ� ��.
template <typename ReturnValue, typename... ARGS>
class CoolTime
{
public:
	// T�� std::function<ReturnValue(ARGS...)>�� ��ȯ �����ؾ� ��.
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

