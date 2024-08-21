#include "Pattern6Action.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossGrowMagicCircle.h"
#include "Boss.h"

void Pattern6Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		bt->GetWorld()->GetCurLevel()->CreateActor<BossGrowMagicCircle>();
		
		// ĳ���� �ִϸ��̼� �ߵ�
		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* CastingAnimationState = bt->GetKey<Boss*>("Boss")->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() != CastingAnimationState)
			abm->SetState(CastingAnimationState);

		started = true;
	}
}

