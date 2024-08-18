#include "Pattern13Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"

#include "BossMeteoCircle.h"
#include "Boss.h"

void Pattern13Action::Run(float dt)
{
	Super::Run(dt);

	BossMeteoCircle* actor = bt->GetWorld()->GetCurLevel()->CreateActor<BossMeteoCircle>();
	Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
	actor->SetLocation(bossLocation.x, bossLocation.y);

}
