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
	
	// TODO: 패턴 시작
	//boss->Pattern6Start();

	// 피리오딕 패턴 쿨타임 재시작
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
