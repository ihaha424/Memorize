#include "Pattern10Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "RazerCircle.h"

void Pattern10Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		bt->GetWorld()->GetCurLevel()->CreateActor<RazerCircle>();

		started = true;
	}
}
