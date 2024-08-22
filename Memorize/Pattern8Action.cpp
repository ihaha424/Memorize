#include "Pattern8Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "Player.h"
#include "Boss.h"
#include "BossProjectile.h"

void Pattern8Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
		Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
		Math::Vector2 toPlayer = (playerLocation - bossLocation);
		toPlayer.Normalize();

		// TODO: 보스 투사체 붙히기
		BossProjectile* proj1 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();

		proj1->SetActor(L"TestResource/Reflection/PatternData/Pattern08_Actor.txt");

		Math::Vector2 vel = toPlayer * 1000.f;

		vel.Normalize();

		proj1->SetPosAndDerection(bossLocation, vel);

		started = true;
		SoundManager::PlayMusic(L"TestResource/Sound/Boss/Sound_Boss_Projectile01.wav");
	}
}
