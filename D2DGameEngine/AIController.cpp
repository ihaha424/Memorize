#include "AIController.h"
#include "BehaviorTree.h"

AIController::AIController(World* _world) : Controller(_world)
{
	SetTickProperties(TICK_UPDATE);
}

AIController::~AIController()
{
}

void AIController::Update(float _dt)
{
	if (brainComponent)
	{
		brainComponent->Update(_dt);
	}
}
