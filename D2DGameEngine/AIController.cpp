#include "AIController.h"
#include "BehaviorTree.h"

AIController::AIController(World* _world) : Controller(_world)
{
	SetTickProperties(TICK_UPDATE);

	bt = new BehaviorTree;
	bt->_controller = this;
}

AIController::~AIController()
{
}

void AIController::Update(float _dt)
{
	bt->UpdateBehaviorTree(_dt);
}
