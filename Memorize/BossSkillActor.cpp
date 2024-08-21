#include "BossSkillActor.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/World.h"
#include "D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"
#include "GPlayerController.h"
#include "Player.h"
#include "Purification.h"
#include "D2DGameEngine/RandomGenerator.h"
#include <utility>

#undef max


BossSkillActor::BossSkillActor(World* _world)
	:Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);

	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->MarkBoundsDirty();
	/*mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);*/
}

BossSkillActor::~BossSkillActor()
{
	//for Purification skill
	auto it1 = GetWorld()->FindActorByType<Player>();
	if (it1 == nullptr)
		return;
	GPlayerController* pController = GetWorld()->FindActorByType<GPlayerController>();
	if (pController == nullptr)
		return;
	Purification* purification = static_cast<Purification*>(pController->FindSkiil(SE_LIGHT, ST_SPECIAL));
	auto it2 = std::find(purification->disfellSkills.begin(), purification->disfellSkills.end(), this);
	if (it2 != purification->disfellSkills.end())
	{
		purification->disfellSkills.erase(it2, purification->disfellSkills.end());
	}
	
}

void BossSkillActor::OnClicked()
{
	//Event -> Player
	//Player.Disfall -> this
}

bool BossSkillActor::Disfell(int _element, class GPlayerController* controller)
{
	if (disfellCommand[dissfellindex] == _element)
	{
		//UI : 디스펠 커맨드 입력 
		controller->OnDoingDisfell->Emit(dissfellindex, _element);

		dissfellindex++;
	
		if (dissfellindex == disfellCommandCount)
		{
			DisfellOneCountAction();
			DisfellAction();
			return false;
		}
		DisfellOneCountAction();
		return true;
	}
	else
	{
		controller->OnDoingDisfell->Emit(-1, 0);
		DisfellFailAction();
		return false;
	}
}

void BossSkillActor::CreateDisfellCommand()
{
	for(int i = 0; i < disfellCommandCount; i++)
		disfellCommand.push_back(Random::Get((int)Dark));

	//for Purification skill
	GPlayerController* pController = static_cast<GPlayerController*>(GetWorld()->FindActorByType<Player>()->GetController());
	if (pController == nullptr)
		return;
	Purification* purification = static_cast<Purification*>(pController->FindSkiil(SE_LIGHT, ST_SPECIAL));
	purification->disfellSkills.push_back(this);
}

void BossSkillActor::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	bm->UpdateBounds();
}
