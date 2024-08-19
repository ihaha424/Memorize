#include "Pattern12Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"

#include "MagicBinding.h"

void Pattern12Action::Run(float dt)
{
	Super::Run(dt);
	if (!started)
	{
		bt->GetWorld()->GetCurLevel()->CreateActor<MagicBinding>();

		started = true;
	}
}
