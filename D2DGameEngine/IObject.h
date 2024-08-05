#pragma once

#include "framework.h"

typedef enum TickPropertyBit {
	TICK_NONE = 0U,
	TICK_PHYSICS = (1U << 1),
	TICK_PRE_UPDATE = (1U << 2),
	TICK_UPDATE = (1U << 3),
	TICK_POST_UPDATE = (1U << 4),
	TICK_RENDER = (1U << 5),
} TickPropertyBit;
using TickPropertyFlags = Flags;

class IObject {
	bool tickEveryFrame{ true };
	float tickInterval{ 0.0 };
	TickPropertyFlags tickPropertyFlags{ TICK_NONE };

public:

	/**
	 * @brief 새로운 오브젝트(액터 또는 컴포넌트)를 생성하여 반환합니다.
	 * @tparam T: IObject를 상속받은 클래스
	 * @return 
	 */
	template <typename T>
	T* CreateDefaultSubobject() const
	{
		static_assert(std::is_base_of<IObject, T>::value, "T must inherit from IObject");

		T* newObject = new T;
		return newObject;
	}

	/**
	 * @brief 매 프레임마다 업데이트 해야할지를 표시합니다.
	 * @param _tickEverySecond True - 매 프레임마다 업데이트 합니다.
	 */
	void EnableTickEverySecond(bool _tickEverySecond) {
		tickEveryFrame = _tickEverySecond;
	}
	/**
	 * @brief 매 프레임마다 업데이트 해야하는지 여부를 반환합니다.
	 * @return True - 매 프레임마다 업데이트 합니다.
	 */
	bool IsTickingEverySecond() {
		return tickEveryFrame;
	}
	/**
	 * @brief 업데이트 인터벌을 설정합니다.
	 * @param _tickInterval 0.0 일 경우 매 프레임 업데이트
	 */
	void SetTickInterval(float _tickInterval) {
		tickInterval = _tickInterval;
	}
	/**
	 * @brief 업데이트 인터벌을 반환합니다.
	 * @return float: 업데이트 인터벌
	 */
	float GetTickInterval() {
		return tickInterval;
	}
	/**
	 * @brief 업데이트 프로퍼티를 설정합니다.
	 * 예시: TICK_PHYSICS | TICK_UPDATE - 픽스 타임 업데이트와 프레임 타임 업데이트에 틱을 실행합니다.
	 * @param _tickPropertyFlags 플레그 비트의 조합
	 */
	void SetTickProperties(TickPropertyFlags _tickPropertyFlags) {
		tickPropertyFlags = _tickPropertyFlags;
	}
	/**
	 * @brief 업데이트 프로퍼티를 반환합니다.
	 * @return 업데이트 프로퍼티 플레그
	 */
	TickPropertyFlags GetTickProperties() {
		tickPropertyFlags;
	}

	virtual void FixedUpdate(float _fixedRate) {}
	virtual void PreUpdate(float _dt) {}
	virtual void Update(float _dt) {}
	virtual void PostUpdate(float _dt) {}
	virtual void Render(class D2DRenderer* _renderer) {}
};



