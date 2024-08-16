#pragma once

#include "common.h"

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

	/**
	 * @brief Ÿ�̾�: Ư�� �ð� ���� ���ε� �Լ��� ��������ش�. ������ �ƴϸ� 1�� ������ ���
	 * ���� ���ǽ�(bool)�� �־ Loop�� ���� ��ų ���� �ִ�.
	 * ���ǽ��� ���̸� ����, �⺻ �� false
	 * @param deltaTime 
	 * @param _condition ���ǽ����� ���̸� �����Ѵ�.
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

