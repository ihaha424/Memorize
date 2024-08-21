#include "TeleportAction.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Boss.h"

#include "D2DGameEngine/Pawn.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/AIController.h"

void TeleportTo::Init()
{
	Super::Init();

	channeling = false;
	castingTime = .8f;
	cooling = false;
	coolingTime = 0.f;
	done = false;
}

bool TeleportTo::IsRunning()
{
	return !done;
}

void TeleportTo::Run(float dt)
{
	if (!started) 
	{
		// Starting animation.
		// 캐스팅 애니메이션 발동
		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* teleportStartAnimationState = bt->GetKey<Boss*>("Boss")->TeleportStartAnimationState;
		if (abm->GetCurrentAnimationScene() != teleportStartAnimationState)
			abm->SetState(teleportStartAnimationState);

		// 채널링 플레그 온
		channeling = true;
		// 시작 플레그 온
		started = true;
	}

	if (channeling)
	{
		castingTime -= dt;
		if (castingTime <= 0.f) {

			Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
			AnimationState* teleportEndAnimationState = bt->GetKey<Boss*>("Boss")->TeleportEndAnimationState;
			if (abm->GetCurrentAnimationScene() != teleportEndAnimationState)
				abm->SetState(teleportEndAnimationState);

			Teleport();

			channeling = false;
			cooling = true;
		}
	}

	if (cooling)
	{
		coolingTime -= dt;
		if (coolingTime <= 0.f)
		{
			Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
			AnimationState* idleAnimationState = bt->GetKey<Boss*>("Boss")->IdleAnimationState;
			if (abm->GetCurrentAnimationScene() != idleAnimationState)
				abm->SetState(idleAnimationState);

			cooling = false;
			done = true;
		}
	}

	// NOTE: Consider using Pathfinder instead
	// GetAIController()->TeleportToLocation(target);
}

void TeleportTo::Teleport()
{
	if (!bt->IsKeySet<Math::Vector2>(key)) {
		failed = true;
		return;
	};

	// Teleport to
	Math::Vector2 target = bt->GetKey<Math::Vector2>(key);
	Math::Vector2 currLocation = GetPawn()->GetLocation();
	Math::Vector2 teleportDelta = target - currLocation;
	GetPawn()->Teleport(teleportDelta);
}
