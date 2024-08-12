#include "BossAIController.h"
#include "Boss.h"
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
	// Create the behavior tree
	BehaviorTree* bt = new BehaviorTree();

	//Keu Declarations가 뭔지 모르겠음??
	Root* root = bt->GetRoot();
	bt->DeclareKey<Boss*>("Boss");
	bt->SetKey<Boss*>("Boss", GetBoss());

	{
		Selector* rootSelector = bt->CreateNode<Selector>();

		{
			Condition* StateDie = bt->CreateNode<Condition>();
			rootSelector->PushBackChild(StateDie);

			bt->DeclareKey<int>("DissFellCount");
			bt->SetKey<int>("DissFellCount", GetBoss()->DissfellCount);
			StateDie->_successCondition = [bt]()->bool
				{
					return (bt->GetKey<int>("DissFellCount") >= 10);
				};
			//Groggy* groggy = bt->CreateNode<Groggy>();
			//StateDie->Wrap(groggy);
		}
		{
			Selector* PatternSelector = bt->CreateNode<Selector>();
			rootSelector->PushBackChild(PatternSelector);
			{
				Condition* periodic_Pattern_Cool_Time = bt->CreateNode<Condition>();
				PatternSelector->PushBackChild(periodic_Pattern_Cool_Time);

				bt->DeclareKey<float>("Periodic_Pattern_Cool_Time");
				bt->SetKey<float>("Periodic_Pattern_Cool_Time", GetBoss()->Periodic_Pattern_Cool_Time);
				periodic_Pattern_Cool_Time->_successCondition = [bt]()->bool
					{
						return (bt->GetKey<int>("Periodic_Pattern_Cool_Time") < 0.f);
					};
				RandomSelector* PatternSelector = bt->CreateNode<RandomSelector>();
			}
		}
	}
}
