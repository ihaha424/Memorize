#pragma once

#include "Event.h"

#include "DamageTypes.h"

#include "HitResult.h"

enum class DamageEventType {
	Default,
	PointDamage,
	RadialDamage,
};

struct DamageEvent : public Event<DamageEvent> {
	DamageEventType damageEventType;

	DamageEvent() : 
		DamageEvent(DamageEventType::Default, DamageType::Default()) {}

	DamageEvent(DamageEventType damageEventType) :
		DamageEvent(damageEventType, DamageType::Default()) {}

	DamageEvent(DamageType damageType) : 
		DamageEvent(DamageEventType::Default, damageType) {}

	DamageEvent(DamageEventType damageEventType, DamageType damageType) :
		damageEventType{ damageEventType }, 
		damageType{ std::make_shared<DamageType>(damageType) } {}

	DamageEventType GetDamageEventType() {
		return damageEventType;
	}

	DamageType GetDamageType() {
		return *damageType.get();
	}

	void SetDamageType(DamageType _damageType) {
		damageType = std::make_shared<DamageType>(_damageType);
	}

private:
	std::shared_ptr<DamageType> damageType;
};

/**
 * @brief 포인트 공격의 이벤트
 */
struct PointDamageEvent : public DamageEvent {
	float damage;					// 데미지 양
	HitResult hitInfo;		// 데미지를 가한 위치와 정보
	DXVec2 shotDirection;	// 삿이 날라온 방향

	PointDamageEvent() : 
		DamageEvent(DamageEventType::PointDamage),
		damage{ 0.f }, 
		hitInfo{}, 
		shotDirection{ DXVec2::Zero } {}

	PointDamageEvent(
		DamageType damageType,
		float damage, 
		HitResult hitInfo, 
		DXVec2 shotDirection) : 
		DamageEvent(DamageEventType::PointDamage, damageType),
		damage{damage},
		hitInfo{hitInfo},
		shotDirection{shotDirection} {}
};

/**
 * @brief 범위 데미지 정보
 */
struct RadialDamageInfo : public DamageEvent {
	float maxDamage;		// 최대 데미지
	float minDamage;		// 최소 데미지
	float damageFalloff;// 거리에 따른 데미지 감소율. 1.0 = 선형, 2.0 = 거리의 제곱
	float innerRadius;	// 맥스 데미지 가하는 반지름
	float outerRadius;	// 데미지를 가하지 않는 가장 바깥쪽의 반지름
	
	RadialDamageInfo() :
		DamageEvent(DamageEventType::RadialDamage),
		maxDamage{ 0.f },
		minDamage{ 0.f },
		damageFalloff{ 0.f },
		innerRadius{ 0.f },
		outerRadius{ 0.f } {}

	RadialDamageInfo(
		DamageType damageType,
		float maxDamage,		
		float minDamage,		
		float damageFalloff,
		float innerRadius,	
		float outerRadius) :
		DamageEvent(DamageEventType::RadialDamage, damageType),
		maxDamage{ maxDamage },
		minDamage{ minDamage },
		damageFalloff{ damageFalloff },
		innerRadius{ innerRadius },
		outerRadius{ outerRadius } {}

	float GetDamageScale(float _distanceFromCenter) {
		if (_distanceFromCenter <= innerRadius) {
			return maxDamage;
		}
		else if (_distanceFromCenter < outerRadius) {
			float ratio = 1.f - (_distanceFromCenter - innerRadius) / (outerRadius - innerRadius);
			float falloff_ratio = std::powf(ratio, damageFalloff);
			return minDamage + (maxDamage - minDamage) * falloff_ratio;
		}
		else {
			return 0.f;
		}
	}

	float GetMaxRadius() {
		return outerRadius;
	}
};

/**
 * @brief 범위 공격 이벤트
 */
struct RadialDamageEvent : public DamageEvent {
	std::vector<HitResult> componentHits;	// 데미지를 가한 위치와 정보
	DXVec2 origin;	// 범위 공격의 원점
	RadialDamageInfo radialDamageInfo;
};


inline DamageEvent DefaultDamage() {
	return DamageEvent();
}
