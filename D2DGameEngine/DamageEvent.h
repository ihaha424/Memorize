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
 * @brief ����Ʈ ������ �̺�Ʈ
 */
struct PointDamageEvent : public DamageEvent {
	float damage;					// ������ ��
	HitResult hitInfo;		// �������� ���� ��ġ�� ����
	DXVec2 shotDirection;	// ���� ����� ����

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
 * @brief ���� ������ ����
 */
struct RadialDamageInfo : public DamageEvent {
	float maxDamage;		// �ִ� ������
	float minDamage;		// �ּ� ������
	float damageFalloff;// �Ÿ��� ���� ������ ������. 1.0 = ����, 2.0 = �Ÿ��� ����
	float innerRadius;	// �ƽ� ������ ���ϴ� ������
	float outerRadius;	// �������� ������ �ʴ� ���� �ٱ����� ������
	
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
