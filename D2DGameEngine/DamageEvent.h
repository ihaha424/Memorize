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
	DamageEventType damageEventType{ DamageEventType::Default };
	DamageType damageType{ DamageType::Default() };
};

/**
 * @brief ����Ʈ ������ �̺�Ʈ
 */
struct PointDamageEvent : public DamageEvent {
	float damage;					// ������ ��
	HitResult hitInfo;		// �������� ���� ��ġ�� ����
	DXVec2 shotDirection;	// ���� ����� ����

	PointDamageEvent() : 
		DamageEvent{ .damageEventType = DamageEventType::PointDamage },
		damage{ 0.f }, 
		hitInfo{}, 
		shotDirection{ DXVec2::Zero } {}

	PointDamageEvent(
		DamageType damageType,
		float damage, 
		HitResult hitInfo, 
		DXVec2 shotDirection) : 
		DamageEvent{ 
			.damageEventType = DamageEventType::PointDamage, 
			.damageType = damageType 
		},
		damage{damage},
		hitInfo{hitInfo},
		shotDirection{shotDirection} {}
};

/**
 * @brief ���� ������ ����
 */
struct RadialDamageInfo : public DamageEvent {
	float maxDamage;		// �ִ� ������
	float minDamage;		// �ּ� ������
	float damageFalloff;// �Ÿ��� ���� ������ ������. 1.0 = ����, 2.0 = �Ÿ��� ����
	float innerRadius;	// �ƽ� ������ ���ϴ� ������
	float outerRadius;	// �������� ������ �ʴ� ���� �ٱ����� ������
	
	RadialDamageInfo() :
		DamageEvent{ .damageEventType = DamageEventType::RadialDamage },
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
		DamageEvent{
			.damageEventType = DamageEventType::PointDamage,
			.damageType = damageType
		},
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
 * @brief ���� ���� �̺�Ʈ
 */
struct RadialDamageEvent : public DamageEvent {
	std::vector<HitResult> componentHits;	// �������� ���� ��ġ�� ����
	DXVec2 origin;	// ���� ������ ����
	RadialDamageInfo radialDamageInfo;
};


inline DamageEvent DefaultDamage() {
	return DamageEvent();
}
