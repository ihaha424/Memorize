#include "BossAIController.h"
#include "Boss.h"
#include "BossBehaviorTree.h"
#include "../D2DGameEngine/Decorator.h"
#include "../D2DGameEngine/Timer.h"
#include "../D2DGameEngine/Debug.h"


BossAIController::BossAIController(class World* _world) : AIController(_world)
{
	SetTickProperties(TICK_UPDATE);
	InitializeBoss();
}

BossAIController::~BossAIController()
{
}

void BossAIController::BeginPlay()
{
	__super::BeginPlay();
}

void BossAIController::Update(float _dt)
{
	__super::Update(_dt);

}

void BossAIController::InitializeBoss()
{
	// Create the behavior tree
	BossBehaviorTree* bt = CreateComponent<BossBehaviorTree>();
	brainComponent = bt;
}
