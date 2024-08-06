#pragma once

#include "framework.h"

struct DamageType {
	bool bCausedByWorld : 1; // 낙하 데미지, 환경물에 의한 데미지 등.
	float damageFalloff; // 거리에 따른 데미지 감소율 1.0 = 선형, 2.0 = 거리의 제곱
	float damageImpulse; // 데미지가 엑터에 입히는 충격량

};