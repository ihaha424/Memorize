#include "Pattern2Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"

#include "Player.h"

#include "BossGrowCircle.h"

void Pattern2Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();

		BossGrowCircle* circle = bt->GetWorld()->GetCurLevel()->CreateActor<BossGrowCircle>();
		circle->SetLocation(playerLocation.x, playerLocation.y);
		circle->SetGrowLess(true);

		started = true;
	}
}
