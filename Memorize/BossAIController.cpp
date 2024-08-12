#include "BossAIController.h"
#include "../D2DGameEngine/BehaviorTree.h"
#include "../D2DGameEngine/Decorator.h"
#include "../D2DGameEngine/Timer.h"
#include "../D2DGameEngine/Debug.h"

BossAIController::BossAIController(class World* _world) : AIController(_world)
{
	SetTickProperties(TICK_UPDATE);
}

BossAIController::~BossAIController()
{
}

void BossAIController::Update(float _dt)
{
	__super::Update(_dt);

}

void BossAIController::InitializeBoss()
{
	//// Create the behavior tree
	//BehaviorTree* bt = new BehaviorTree();

	////Keu Declarations가 뭔지 모르겠음??
	//Root* root = bt->GetRoot();

	//Selector* rootSelector = bt->CreateNode<Selector>();
	//Condition* StateDie = bt->CreateNode<Condition>();
	//StateDie->_successCondition = [bt]()bool
	//{
	//}
}
