#include "Pattern6Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "BossGrowMagicCircle.h"

void Pattern6Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		bt->GetWorld()->GetCurLevel()->CreateActor<BossGrowMagicCircle>();

		started = true;
	}
}

