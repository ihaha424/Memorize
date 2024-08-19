#include "PatternAction.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool PatternAction::IsRunning()
{
	if (elapsedTime >= patternInterval)
	{
		elapsedTime = 0.f;
		GetBehaviorTree()->GetKey<Boss*>("Boss")->Periodic_Pattern_Cool_Time = GetCooldown();
		GetBehaviorTree()->GetKey<Boss*>("Boss")->Pattern_Delay = GetTreeCooldown();
		return false;
	}
	else
	{
		return true;
	}
}
