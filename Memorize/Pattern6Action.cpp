#include "Pattern6Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "Boss.h"
#include "BossGrowMagicCircle.h"

void Pattern6Action::Run(float dt)
{
	Super::Run(dt);
	Boss* boss = GetBehaviorTree()->GetKey<Boss*>("Boss");
	
	// TODO: ���� ����
	//boss->Pattern6Start();

	// �Ǹ����� ���� ��Ÿ�� �����
	boss->Periodic_Pattern_Cool_Time = GetCooldown();
}

void Pattern6Action::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_ActionNode.txt");
	reflectionResource->ParsingFile(0, delay, patternCooldownTime, posCircle, bossSpeed);
	patternInterval = delay;
}

void Pattern6Action::ReflectionOut()
{
}
