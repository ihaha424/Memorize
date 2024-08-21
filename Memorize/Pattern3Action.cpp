#include "Pattern3Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossProjectile.h"
#include "Player.h"
#include "Boss.h"

void Pattern3Action::Run(float dt)
{
	Super::Run(dt);

	if (!started)
	{
		Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
		Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
		Math::Vector2 toPlayer = (playerLocation - bossLocation);
		toPlayer.Normalize();

		// 보스 투사체 붙히기
		BossProjectile* proj1 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();
		BossProjectile* proj2 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();
		BossProjectile* proj3 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();

		proj1->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");
		proj2->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");
		proj3->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");

		Math::Matrix rotate1 = Math::Matrix::CreateRotationZ(Math::DegreeToRadian(20.f));
		Math::Matrix rotate3 = Math::Matrix::CreateRotationZ(Math::DegreeToRadian(-20.f));

		Math::Vector2 vel = toPlayer * 800.f;

		Math::Vector2 tempDerction1 = Math::Vector2::Transform(vel, rotate1);
		tempDerction1.Normalize();
		Math::Vector2 tempDerction2 = vel;
		tempDerction2.Normalize();
		Math::Vector2 tempDerction3 = Math::Vector2::Transform(vel, rotate3);
		tempDerction3.Normalize();

		proj1->SetPosAndDerection(bossLocation, tempDerction1);
		proj2->SetPosAndDerection(bossLocation, tempDerction2);
		proj3->SetPosAndDerection(bossLocation, tempDerction3);
		
		started = true;
	}
}
