#include "Pattern3Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossProjectile.h"
#include "Player.h"
#include "Boss.h"

void Pattern3Action::Run(float dt)
{
	Super::Run(dt);

	Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
	Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
	Math::Vector2 toPlayer = (playerLocation - bossLocation);
	toPlayer.Normalize();

	// TODO: 보스 투사체 붙히기
	BossProjectile* proj1 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();
	BossProjectile* proj2 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();
	BossProjectile* proj3 = bt->GetWorld()->GetCurLevel()->CreateActor<BossProjectile>();

	proj1->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");
	proj2->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");
	proj3->SetActor(L"TestResource/Reflection/PatternData/Pattern03_Actor.txt");

	proj1->SetLocation(bossLocation.x, bossLocation.y);
	proj2->SetLocation(bossLocation.x, bossLocation.y);
	proj3->SetLocation(bossLocation.x, bossLocation.y);

	Math::Matrix rotate1 = Math::Matrix::CreateRotationZ(20.f);
	Math::Matrix rotate3 = Math::Matrix::CreateRotationZ(-20.f);
	
	Math::Vector2 vel = toPlayer * 800.f;

	proj1->SetVelocity(Math::Vector2::Transform(vel, rotate1));
	proj2->SetVelocity(vel);
	proj3->SetVelocity(Math::Vector2::Transform(vel, rotate3));
}
