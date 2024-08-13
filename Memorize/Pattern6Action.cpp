#include "Pattern6Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool Pattern6Action::IsRunning()
{
	return false;
}

void Pattern6Action::Run(float dt)
{
	Boss* boss = GetBehaviorTree()->GetKey<Boss*>("Boss");
	// TODO: 패턴 시작
	// boss->Pattern6Start();

	// 피리오딕 패턴 쿨타임 재시작
	boss->Periodic_Pattern_Cool_Time = GetCoolTime();
}
