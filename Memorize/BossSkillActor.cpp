#include "BossSkillActor.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"

BossSkillActor::BossSkillActor(World* _world)
	:Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);

	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->MarkBoundsDirty();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);
	
}

BossSkillActor::~BossSkillActor()
{
}

void BossSkillActor::OnClicked()
{
	//Event -> Player
	//Player.Disfall -> this
}

void BossSkillActor::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	bm->UpdateBounds();
}
