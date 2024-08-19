#include "WaitForDelay.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool WaitForDelay::IsRunning()
{
	Boss* boss = bt->GetKey<Boss*>("Boss");
	return boss->Pattern_Delay > 0.f;
}

void WaitForDelay::Run(float dt)
{
	Boss* boss = bt->GetKey<Boss*>("Boss");
	if (boss->Periodic_Pattern_Cool_Time <= 0.f) return;
	boss->Periodic_Pattern_Cool_Time -= dt;
}
