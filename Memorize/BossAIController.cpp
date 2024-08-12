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
			//	Groggy
			Condition* StateDie = bt->CreateNode<Condition>();
			rootSelector->PushBackChild(StateDie);

			bt->DeclareKey<int>("DissFellCount");
			bt->SetKey<int>("DissFellCount", GetBoss()->DissfellCount);
			StateDie->_successCondition = [bt]()->bool
				{
					return (bt->GetKey<int>("DissFellCount") >= 10);
				};
			{
				//Groggy
			}
		}
		{
			// BehaviorSelector
			Selector* BehaviorSelector = bt->CreateNode<Selector>();
			rootSelector->PushBackChild(BehaviorSelector);
			bt->DeclareKey<float>("Detection_Range");
			bt->SetKey<float>("Detection_Range", GetBoss()->Detection_Range);
			bt->DeclareKey<float>("Avoidance_Range");
			bt->SetKey<float>("Avoidance_Range", GetBoss()->Avoidance_Range);

			{
				//	Periodic_Pattern
				Condition* periodic_Pattern_Cool_Time = bt->CreateNode<Condition>();
				BehaviorSelector->PushBackChild(periodic_Pattern_Cool_Time);

				bt->DeclareKey<float>("Periodic_Pattern_Cool_Time");
				bt->SetKey<float>("Periodic_Pattern_Cool_Time", GetBoss()->Periodic_Pattern_Cool_Time);
				periodic_Pattern_Cool_Time->_successCondition = [bt]()->bool
					{
						return (bt->GetKey<int>("Periodic_Pattern_Cool_Time") < 0.f);
					};
				{
					RandomSelector* BehaviorSelector = bt->CreateNode<RandomSelector>();
					periodic_Pattern_Cool_Time->Wrap(BehaviorSelector);
					{
						{
							//Pattern06
							//periodic_Pattern_Cool_Time = Pattern06->DelayTime
						}
						{
							//Pattern07
							//periodic_Pattern_Cool_Time = Pattern07->DelayTime
						}
						{
							//Pattern10
							//periodic_Pattern_Cool_Time = Pattern10->DelayTime
						}
					}
				}

				//	MoveAction
				Condition* moveActionCondition = bt->CreateNode<Condition>();
				BehaviorSelector->PushBackChild(moveActionCondition);
				moveActionCondition->_successCondition = [bt]()->bool
					{
						return (bt->GetKey<float>("Detection_Range") < bt->GetKey<float>("Palyer_Distance"));
					};
				{
					//MoveACtion
				}

				//	Telproting
				Condition* telprotingCondition = bt->CreateNode<Condition>();
				BehaviorSelector->PushBackChild(telprotingCondition);
				telprotingCondition->_successCondition = [bt]()->bool
					{
						return (bt->GetKey<float>("Palyer_Distance") < bt->GetKey<float>("Avoidance_Range"));
					};
				{
					//Telproting
				}

				//	BossPhase
				Selector* BossPhase = bt->CreateNode<Selector>();
				BehaviorSelector->PushBackChild(BossPhase);
				bt->DeclareKey<float>("Boss_HP");
				bt->SetKey<float>("Boss_HP", GetBoss()->hp);
				{
					Condition* BossPhase_One = bt->CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_One);
					BossPhase_One->_successCondition = [bt]()->bool
						{
							return (75 < bt->GetKey<float>("Boss_HP") && bt->GetKey<float>("Boss_HP") <= 100);
						};
					{
						// Boss Phase One
						RandomSelector* Phase_One_Selector = bt->CreateNode<RandomSelector>();
						BossPhase_One->Wrap(Phase_One_Selector);
						{
							//Pattern02
							//Wait* Pattern02.DelayTime

						}
						{
							//	Phase_One_2
							Sequence* Phase_One_2 = bt->CreateNode<Sequence>();
							Phase_One_Selector->PushBackChild(Phase_One_2);
							{
								//Pattern05
								//Pattern03
								//Pattern04
								//Wait* Pattern04.DelayTime
							}
						}
						{
							//	Phase_One_3
							Sequence* Phase_One_3 = bt->CreateNode<Sequence>();
							Phase_One_Selector->PushBackChild(Phase_One_3);
							{
								//Pattern05
								//Pattern08
								//Wait* Pattern08.DelayTime
							}
						}
						{
							//Pattern01
							//Wait* Pattern08.DelayTime
						}
					}
				}
				
				{
					Condition* BossPhase_Two = bt->CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_Two);
					BossPhase_Two->_successCondition = [bt]()->bool
						{
							return (25 < bt->GetKey<float>("Boss_HP") && bt->GetKey<float>("Boss_HP") <= 75);
						};
					{
						Selector* Phase_Pattern_Select = bt->CreateNode<Selector>();
						BossPhase_Two->Wrap(Phase_Pattern_Select);
						bt->DeclareKey<float>("Phase_Pattern_Cool_Time");
						bt->SetKey<float>("Phase_Pattern_Cool_Time", GetBoss()->Phase_Pattern_Cool_Time);
						{
							// 2 Phase Periodic Pattern
							Condition* Phase_Two_Periodic = bt->CreateNode<Condition>();
							Phase_Pattern_Select->PushBackChild(Phase_Two_Periodic);
							Phase_Two_Periodic->_successCondition = [bt]()->bool
								{
									return (bt->GetKey<float>("Phase_Pattern_Cool_Time") < 0.f);
								};
							{
								//Pattern09
								// Phase_Pattern_Cool_Time = Pattern19->DelayTime
							}
						}
						{
							// Boss Phase Two
							RandomSelector* Phase_Two_Selector = bt->CreateNode<RandomSelector>();
							Phase_Pattern_Select->PushBackChild(Phase_Two_Selector);
							{
								//	Phase_Two_1
								Sequence* Phase_Two_1 = bt->CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_1);
								{
									//Pattern02
									//Pattern02
									//Wait* Pattern02.DelayTime
								}
							}
							{
								//	Phase_Two_2
								Sequence* Phase_Two_2 = bt->CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_2);
								{
									//Pattern03
									//Pattern04
									//Wait* Pattern04.DelayTime
								}
							}
							{
								//	Phase_Two_3
								Sequence* Phase_Two_3 = bt->CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_3);
								{
									//Pattern05
									//Pattern08
									//Wait* Pattern08.DelayTime

								}
							}
							{
								//	Phase_Two_4
								Sequence* Phase_Two_4 = bt->CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_4);
								{
									//Pattern05
									//Pattern03
									//Wait* Pattern04.DelayTime

								}
							}
						}
					}
				}

				{
					Condition* BossPhase_Three = bt->CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_Three);
					BossPhase_Three->_successCondition = [bt]()->bool
						{
							return (bt->GetKey<float>("Boss_HP") <= 25);
						};
					{
						Selector* Phase_Pattern_Select = bt->CreateNode<Selector>();
						BossPhase_Three->Wrap(Phase_Pattern_Select);
						{
							// 3 Phase Periodic Pattern 1
							Condition* Phase_Three_Periodic = bt->CreateNode<Condition>();
							Phase_Pattern_Select->PushBackChild(Phase_Three_Periodic);
							Phase_Three_Periodic->_successCondition = [bt]()->bool
								{
									return (bt->GetKey<float>("Phase_Pattern_Cool_Time") < 0.f);
								};
							{
								// Phase_Three_Periodic_Selector
								RandomSelector* Phase_Three_Periodic_Selector = bt->CreateNode<RandomSelector>();
								Phase_Pattern_Select->PushBackChild(Phase_Three_Periodic_Selector);
								{
									// 3 Phase Periodic Pattern 1
									Sequence* Phase_Three_1 = bt->CreateNode<Sequence>();
									Phase_Three_Periodic_Selector->PushBackChild(Phase_Three_1);
									{
										//Pattern12
										{
											// Phase_Three_Periodic_1_RandomSelecor
											RandomSelector* Phase_Three_Periodic_1_RandomSelecor = bt->CreateNode<RandomSelector>();
											Phase_Three_1->PushBackChild(Phase_Three_Periodic_1_RandomSelecor);
											{
												//Pattern08
												// Phase_Pattern_Cool_Time = Pattern12->DelayTime
											}
											{
												//Pattern09
												// Phase_Pattern_Cool_Time = Pattern12->DelayTime
											}
										}
									}
								}
								{
									// 3 Phase Periodic Pattern 2
									Sequence* Phase_Three_2 = bt->CreateNode<Sequence>();
									Phase_Three_Periodic_Selector->PushBackChild(Phase_Three_2);
									{
										//Pattern13
										//Pattern02
										//Pattern02
										//Pattern02
										// Phase_Pattern_Cool_Time = Pattern13->DelayTime
									}
								}
							}

							
						}
						{
							// Boss Phase Three
							RandomSelector* Phase_Three_Selector = bt->CreateNode<RandomSelector>();
							Phase_Pattern_Select->PushBackChild(Phase_Three_Selector);
							{
								//	Phase_Three_1
								Sequence* Phase_Three_1 = bt->CreateNode<Sequence>();
								Phase_Three_Selector->PushBackChild(Phase_Three_1);
								{
									//Pattern09
									//Pattern02
									//Pattern02
									//Wait* Pattern09.DelayTime
								}
							}
							{
								//	Phase_Three_2
								Sequence* Phase_Three_2 = bt->CreateNode<Sequence>();
								Phase_Three_Selector->PushBackChild(Phase_Three_2);
								{
									//Pattern02
									//Pattern04
									//Wait* Pattern04.DelayTime
								}
							}
							{
								//	Phase_Three_3
								Sequence* Phase_Three_3 = bt->CreateNode<Sequence>();
								Phase_Three_Selector->PushBackChild(Phase_Three_3);
								{
									//Pattern11
									//Pattern04
									//Pattern04
									//Pattern04
									//Wait* Pattern11.DelayTime
								}
							}
						}
					}
				}
			}
		}
	}
}
