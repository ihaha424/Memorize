#include "BossSkillActor.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/RandomGenerator.h"

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

bool BossSkillActor::Disfell(int _element)
{
	if (disfellCommand[dissfellindex] == _element)
	{
		dissfellindex++;
		if (dissfellindex == disfellCommandCount)
		{
			DisfellAction();
			return false;
		}
		DisfellOneCountAction();
		return true;
	}
	else
	{
		DisfellFailAction();
		return false;
	}
}

void BossSkillActor::CreateDisfellCommand()
{
	for(int i = 0; i < disfellCommandCount; i++)
		disfellCommand.push_back(Random::Get((int)Dark));
}

void BossSkillActor::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	bm->UpdateBounds();
}
