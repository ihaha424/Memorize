#include "Pattern2Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"

#include "Player.h"

#include "BossGrowMagicCircle.h"

void Pattern2Action::Run(float dt)
{
	Super::Run(dt);

	Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();

	BossGrowMagicCircle* circle = bt->GetWorld()->GetCurLevel()->CreateActor<BossGrowMagicCircle>();
	circle->SetLocation(playerLocation.x, playerLocation.y);
}
