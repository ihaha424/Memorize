#include "GroggyAction.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/AnimationEffect.h"

#include "Boss.h"

bool GroggyAction::IsRunning()
{
    return groggyTimer <= 0.f;
}

void GroggyAction::Run(float dt)
{
	// TODO;
	if (!started) {
		// Set groggy animation true 

		AnimationEffect* DestoryProjectileEffect = bt->GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/Boss_Meteor/BOSS_Skill_Meteor_Explosion.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.040f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.5f);

		started = true;
	}
	
	groggyTimer -= dt;

	if (groggyTimer <= 0.f)
	{
		// 그로기 종료
		Boss* boss = bt->GetKey<Boss*>("Boss");
		boss->DissfellCount = 0;
	}
}
