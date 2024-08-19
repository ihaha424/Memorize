#include "Pattern4Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "Player.h"
#include "Boss.h"
#include "BossProjectile.h"

void Pattern4Action::Run(float dt)
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
		BossProjectile* proj2 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();

		proj1->SetActor(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");
		proj2->SetActor(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");

		proj1->SetLocation(bossLocation.x, bossLocation.y);
		proj2->SetLocation(bossLocation.x, bossLocation.y);

		Math::Matrix rotate1 = Math::Matrix::CreateRotationZ(10.f);
		Math::Matrix rotate2 = Math::Matrix::CreateRotationZ(-10.f);

		Math::Vector2 vel = toPlayer * 800.f;

		proj1->SetVelocity(Math::Vector2::Transform(vel, rotate1));
		proj2->SetVelocity(Math::Vector2::Transform(vel, rotate2));

		started = true;
	}
}
