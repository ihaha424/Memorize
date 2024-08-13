#include "BossSkillActor.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"

BossSkillActor::BossSkillActor(World* _world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = bm = CreateComponent<BitmapComponent>();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

	mv->SetStatus(OS_INACTIVE);
	bm->SetStatus(OS_INACTIVE);
}

BossSkillActor::~BossSkillActor()
{
}
