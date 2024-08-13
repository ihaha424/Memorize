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
	// TODO: ���� ����
	// boss->Pattern6Start();

	// �Ǹ����� ���� ��Ÿ�� �����
	boss->Periodic_Pattern_Cool_Time = GetCoolTime();
}
