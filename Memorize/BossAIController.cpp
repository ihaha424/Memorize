#include "BossAIController.h"
#include "../D2DGameEngine/BehaviorTree.h"
#include "../D2DGameEngine/Decorator.h"
#include "../D2DGameEngine/Timer.h"
#include "../D2DGameEngine/Debug.h"

BossAIController::BossAIController(class World* _world) : AIController(_world)
{
}

BossAIController::~BossAIController()
{
}

void BossAIController::Update(float _dt)
{
	__super::Update(_dt);

}
