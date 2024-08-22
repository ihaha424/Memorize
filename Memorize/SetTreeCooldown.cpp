#include "SetTreeCooldown.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool SetTreeCoolTime::IsRunning()
{
	return false;
}

void SetTreeCoolTime::Run(float dt)
{
	GetBehaviorTree()->GetKey<Boss*>("Boss")->Pattern_Delay = coolTime;
}
