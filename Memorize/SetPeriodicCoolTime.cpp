#include "SetPeriodicCoolTime.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool SetPeriodicCoolTime::IsRunning()
{
    return false;
}

void SetPeriodicCoolTime::Run(float dt)
{
	bt->GetKey<Boss*>("Boss")->Periodic_Pattern_Cool_Time = coolTime;
}
