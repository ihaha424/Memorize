#include "Pattern11Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossChaseCircle.h"
#include "Player.h"

void Pattern11Action::Run(float dt)
{
	Super::Run(dt);

	BossChaseCircle* actor = bt->GetWorld()->GetCurLevel()->CreateActor<BossChaseCircle>();
	Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
	actor->SetLocation(playerLocation.x, playerLocation.y);
}
