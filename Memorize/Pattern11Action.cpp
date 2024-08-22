#include "Pattern11Action.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossChaseCircle.h"
#include "Player.h"
#include "Boss.h"

void Pattern11Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		BossChaseCircle* actor = bt->GetWorld()->GetCurLevel()->CreateActor<BossChaseCircle>();
		Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
		actor->SetLocation(bossLocation.x, bossLocation.y);

		// 캐스팅 애니메이션 발동
		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* CastingAnimationState = bt->GetKey<Boss*>("Boss")->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() != CastingAnimationState)
			abm->SetState(CastingAnimationState);

		started = true;
		SoundManager::PlayMusic(L"TestResource/Sound/Boss/Boss_Pattern_Activated.wav");
	}
}
