#include "PatternAction.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

bool PatternAction::IsRunning()
{
	if (elapsedTime >= patternInterval)
	{
		elapsedTime = 0.f;
		GetBehaviorTree()->GetKey<Boss*>("Boss")->Periodic_Pattern_Cool_Time += GetCooldown();
		GetBehaviorTree()->GetKey<Boss*>("Boss")->Pattern_Delay += GetTreeCooldown();

		//// 애니메이션 초기화
		//Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		//AnimationState* IdleAnimationState = bt->GetKey<Boss*>("Boss")->IdleAnimationState;
		//AnimationState* CastingAnimationState = bt->GetKey<Boss*>("Boss")->CastingAnimationState;
		//if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		//	abm->SetState(IdleAnimationState);

		return false;
	}
	else
	{
		return true;
	}
}
