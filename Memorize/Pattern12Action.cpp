#include "Pattern12Action.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "Boss.h"

#include "MagicBinding.h"

void Pattern12Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		bt->GetWorld()->GetCurLevel()->CreateActor<MagicBinding>();

		// 캐스팅 애니메이션 발동
		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* CastingAnimationState = bt->GetKey<Boss*>("Boss")->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() != CastingAnimationState)
			abm->SetState(CastingAnimationState);

		started = true;
		SoundManager::PlayMusic(L"TestResource/Sound/Boss/Boss_Pattern_Activated.wav");
	}
}
