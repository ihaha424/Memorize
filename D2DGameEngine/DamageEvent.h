#pragma once

#include "Event.h"

#include "DamageType.h"

#include "HitResult.h"

struct DamageEvent : public Event<DamageEvent> {
	DamageType damageType;
};

/**
 * @brief 포인트 공격의 이벤트
 */
struct PointDamageEvent : public DamageEvent {
	float damage;					// 데미지 양
	HitResult hitInfo;		// 데미지를 가한 위치와 정보
	DXVec2 shotDirection;	// 삿이 날라온 방향
};

/**
 * @brief 범위 데미지 정보
 */
struct RadialDamageInfo {
	float maxDamage;		// 최대 데미지
	float minDamage;		// 최소 데미지
	float damageFalloff;// 거리에 따른 데미지 감소율. 1.0 = 선형, 2.0 = 거리의 제곱
	float innerRadius;	// 맥스 데미지 가하는 반지름
	float outerRadius;	// 데미지를 가하지 않는 가장 바깥쪽의 반지름
	
	float GetDamageScale(float _distanceFromCenter) {
		if (_distanceFromCenter <= innerRadius) {
			return MaxDamage;
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
