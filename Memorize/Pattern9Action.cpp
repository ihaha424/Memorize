#include "Pattern9Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossThroughProjectileMagicCircle.h"
#include "Boss.h"

void Pattern9Action::Run(float dt)
{
	Super::Run(dt);

	BossThroughProjectileMagicCircle* actor= bt->GetWorld()->GetCurLevel()->CreateActor<BossThroughProjectileMagicCircle>();
	Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
	actor->SetLocation(bossLocation.x, bossLocation.y);
}
