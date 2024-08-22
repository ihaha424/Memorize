#include "GroggyAction.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/AnimationEffect.h"

#include "Boss.h"

bool GroggyAction::IsRunning()
{
    return groggyTimer > 0.f;
}

void GroggyAction::Run(float dt)
{
	// TODO;
	if (!started) {
		// Set groggy animation true 

		Boss* boss = bt->GetKey<Boss*>("Boss");
		Math::Vector2 bossLocation = boss->GetLocation();

		AnimationEffect* groggyEffect = bt->GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		groggyEffect->SetSprite(L"TestResource/Boss/BossMotions/Boss_groggy.png");
		groggyEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 200, 0, 0, 0, 0);
		groggyEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 1.f / 36.f });
		groggyEffect->GetAnimationBitmapComponent()->SetLoop(true);
		groggyEffect->GetAnimationBitmapComponent()->Trigger(true);
		groggyEffect->SetAliveTime(8.f);
		groggyEffect->SetLocation(bossLocation.x, bossLocation.y - 200.f);

		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* GroggyAnimationState = bt->GetKey<Boss*>("Boss")->GroggyAnimationState;
		abm->SetState(GroggyAnimationState);

		started = true;
	}
	
	groggyTimer -= dt;

	if (groggyTimer <= 0.f)
	{
		// 그로기 종료
		Boss* boss = bt->GetKey<Boss*>("Boss");
		boss->DissfellCount = 0;

		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* IdleAnimationState = bt->GetKey<Boss*>("Boss")->IdleAnimationState;
		abm->SetState(IdleAnimationState);
	}
}
