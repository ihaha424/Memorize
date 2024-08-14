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
	bool bCausedByWorld : 1; // ���� ������, ȯ�湰�� ���� ������ ��.
	EDamageType damageType;
	float damageFalloff; // �Ÿ��� ���� ������ ������ 1.0 = ����, 2.0 = �Ÿ��� ����
	float damageImpulse; // �������� ���Ϳ� ������ ��ݷ�

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
