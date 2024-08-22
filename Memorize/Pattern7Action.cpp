#include "Pattern7Action.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossRazer.h"
#include "Player.h"
#include "Boss.h"

void Pattern7Action::Run(float dt)
{
	Super::Run(dt);

	if (!started)
	{
		Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
		Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
		Math::Vector2 toPlayer = (playerLocation - bossLocation);
		toPlayer.Normalize();

		BossRazer* razer1 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer2 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer3 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer4 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer5 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer6 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
		BossRazer* razer7 = bt->GetWorld()->GetCurLevel()->CreateActor<BossRazer>();

		BossRazer* razers[]{
			razer1, razer2, razer3, razer4, razer5, razer6, razer7
		};

		Math::Vector2 locationOffset;
		if (toPlayer.x <= 0)
		{
			locationOffset = { -100.f, 200.f };
		}
		else
		{
			locationOffset = { 100.f, 200.f };
			for (BossRazer* razer : razers)
			{
				razer->SetScale(-1, 1);
			}
		}

		for (int i = -3; i < 4; ++i) {
			Math::Vector2 offset{
				locationOffset.x,
				locationOffset.y * i
			};
			Math::Vector2 location = bossLocation + offset;
			razers[i + 3]->SetLocation(location.x, location.y);
		}

		// 캐스팅 애니메이션 발동
		Animator* abm = bt->GetKey<Boss*>("Boss")->abm;
		AnimationState* CastingAnimationState = bt->GetKey<Boss*>("Boss")->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() != CastingAnimationState)
			abm->SetState(CastingAnimationState);

		started = true;
		SoundManager::PlayMusic(L"TestResource/Sound/Boss/Boss_Pattern_Activated.wav");
		SoundManager::PlayMusic(L"TestResource/Sound/Boss/Sound_Boss_Laser.wav");
	}
}
