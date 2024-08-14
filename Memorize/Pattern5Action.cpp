#include "Pattern5Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "Boss.h"

void Pattern5Action::Run(float dt)
{
	Super::Run(dt);
	Boss* boss = bt->GetKey<Boss*>("Boss");

	if ((int)elapsedTime % 1 == 0) {
	// Cast the skill

	}



}
