#pragma once

#include "framework.h"

enum class EDamageType
{
	Default,
	Fire,
	Water,
	Darkness,
	Brightness,
};

struct DamageType {
	bool bCausedByWorld : 1; // 낙하 데미지, 환경물에 의한 데미지 등.
	EDamageType damageType;
	float damageFalloff; // 거리에 따른 데미지 감소율 1.0 = 선형, 2.0 = 거리의 제곱
	float damageImpulse; // 데미지가 엑터에 입히는 충격량

	static DamageType Default() {
		return DamageType{ false, EDamageType::Default, 1.0, 0.0 };
	}
};


//struct DamageType_Fire : public DamageType {
//
//};

struct DamageType_Darkness : public DamageType
{
	DamageType_Darkness() : DamageType{ false, EDamageType::Darkness, 1.0, 0.0 } {}

	DamageType_Darkness(bool bCausedByWorld, float damageFalloff, float damageImpulse) :
		DamageType{ bCausedByWorld, EDamageType::Darkness, damageFalloff, damageImpulse } {}

	static DamageType_Darkness Default() {
		return DamageType_Darkness();
	}
};

// Actor TakeDamage;
