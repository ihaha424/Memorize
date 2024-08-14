#include "Pattern6Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

void Pattern6Action::Run(float dt)
{
	Super::Run(dt);
	Boss* boss = GetBehaviorTree()->GetKey<Boss*>("Boss");
	// TODO: 패턴 시작
	// boss->Pattern6Start();

	// 피리오딕 패턴 쿨타임 재시작
	boss->Periodic_Pattern_Cool_Time = GetCooldown();
}
