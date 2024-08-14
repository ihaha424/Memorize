#include "Pattern6Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

void Pattern6Action::Run(float dt)
{
	Super::Run(dt);
	Boss* boss = GetBehaviorTree()->GetKey<Boss*>("Boss");
	// TODO: ���� ����
	// boss->Pattern6Start();

	// �Ǹ����� ���� ��Ÿ�� �����
	boss->Periodic_Pattern_Cool_Time = GetCooldown();
}
