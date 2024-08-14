#include "BossBehaviorTree.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "D2DGameEngine/AIController.h"
#include "Boss.h"
#include "Player.h"

#include "BossActionNodes.h"

BossBehaviorTree::BossBehaviorTree(Actor* _aiOwner) : BehaviorTree(_aiOwner)
{
	SetTickProperties(TICK_UPDATE);

	boss = static_cast<Boss*>(GetAIOwner()->GetPawn());
	
	BuildBehaviorTree();
}

void BossBehaviorTree::BeginPlay()
{
	Super::BeginPlay();

	// ÇÃ·¹ÀÌ¾î Ã£¾Æ¼­ Æ÷ÀÎÅÍ·Î ³Ñ±è. Key·Î Á¢±Ù °¡´É
	DeclareKey<Player*>("Player");
	SetKey<Player*>("Player", GetWorld()->FindActorByType<Player>());
	
}

void BossBehaviorTree::Update(float _dt)
{
}

void BossBehaviorTree::BuildBehaviorTree()
{
	//Keu Declarations°¡ ¹ºÁö ¸ð¸£°ÚÀ½??
	Root* root = GetRoot();
	DeclareKey<Boss*>("Boss");
	SetKey<Boss*>("Boss", boss);
	{
		Selector* rootSelector = CreateNode<Selector>();

		{	// -> Root select 1
			//	Groggy
			Condition* groggyCondition = CreateNode<Condition>();
			rootSelector->PushBackChild(groggyCondition);

			// TODO: force if?
			groggyCondition->_successCondition = [this]() {
				Boss* boss = GetKey<Boss*>("Boss");
				return (boss->DissfellCount >= 10);
				};
			{
				//Groggy
				Sequence* groggySequence = CreateNode<Sequence>();
				groggySequence->PushBackChild(groggyCondition);
				{
					// GroggyAction
					GroggyAction* groggyAction = CreateNode<GroggyAction>();
					groggySequence->PushBackChild(groggyAction);

					// Wait for 10
					Wait* groggyWaitFor10Seconds = CreateNode<Wait>(5);
					groggySequence->PushBackChild(groggyWaitFor10Seconds);
				}
			}
		}
		{	// -> Root select 2
			// BehaviorSelector
			Selector* BehaviorSelector = CreateNode<Selector>();
			rootSelector->PushBackChild(BehaviorSelector);

			{	// -> BehaviorSelector 1
				// Periodic_Pattern
				Condition* IsCooledDown = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(IsCooledDown);

				IsCooledDown->_successCondition = [this]() {
					Boss* boss = GetKey<Boss*>("Boss");
					return boss->Periodic_Pattern_Cool_Time <= 0.f;
				};

				{
					RandomSelector* BehaviorSelector = CreateNode<RandomSelector>();
					IsCooledDown->Wrap(BehaviorSelector);
					{
						{	// -> BehaviorSelector 1
							//Pattern06
							//periodic_Pattern_Cool_Time = Pattern06->DelayTime
							Sequence* Pattern6Sequence = CreateNode<Sequence>();

							MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
							moveToCenter->SetAcceptableRadius(10.f);
							moveToCenter->SetSpeed(900.f);
							moveToCenter->SetDestination(/*TODO:센터 값 조절 요*/{ 2014, 1050 });

							Pattern6Action* pattern6Action = CreateNode<Pattern6Action>();
							pattern6Action->SetCooldown(15.f);
							pattern6Action->SetPatternInterval(11.f);

							Pattern6Sequence->PushBackChildren({ moveToCenter, pattern6Action });
						}
						{	// -> BehaviorSelector 2
							//Pattern07
							//periodic_Pattern_Cool_Time = Pattern07->DelayTime
							Sequence* Pattern7Sequence = CreateNode<Sequence>();

							DeclareKey<Math::Vector2>("Pattern7Destination");
							Primer* setDestination = CreateNode<Primer>();
							setDestination->_action = [this]() {
								Math::Vector2 playerLocation = GetKey<Player*>("Player")->GetLocation();

								Math::Vector2 destinationCandidate1 = playerLocation + Math::Vector2{ 800, 0 };
								Math::Vector2 destinationCandidate2 = playerLocation - Math::Vector2{ 800, 0 };

								// TODO: 이동 가능한지 확인
								// 맵 마름모 꼴이니까 변마다 Line 만들고
								// Line{destination, BossCurrLoc} 가지고 
								// 교점 체크
								// -> 없으면 가능

								// 만약 둘다 가능하면 랜덤으로 결정
								// SetKey<Math::Vector2>("Pattern7Destination", destination);

								// 만약 둘다 안되면
								// ResetKey<Math::Vector2>("Pattern7Destination");
							};

							// Selector to find the correct position.
							MoveTo* moveToPlayer = CreateNode<MoveTo>();
							moveToPlayer->SetObserveLocationKey("Pattern7Destination");
							moveToPlayer->SetAcceptableRadius(10.f);
							moveToPlayer->SetSpeed(900.f);

							setDestination->Wrap(moveToPlayer);

							Pattern7Action* pattern7Action = CreateNode<Pattern7Action>();
							pattern7Action->SetCooldown(20.f);
							pattern7Action->SetPatternInterval(5.f);

							Pattern7Sequence->PushBackChildren({ setDestination, pattern7Action });
						}
						{	// -> BehaviorSelector 3
							//Pattern10
							//periodic_Pattern_Cool_Time = Pattern10->DelayTime
							Sequence* Pattern10Sequence = CreateNode<Sequence>();

							MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
							moveToCenter->SetAcceptableRadius(10.f);
							moveToCenter->SetSpeed(900.f);
							moveToCenter->SetDestination(/*TODO:센터 값 조절 요*/{ 2014, 1050 });

							Pattern10Action* pattern10Action = CreateNode<Pattern10Action>();
							pattern10Action->SetCooldown(20.f);
							pattern10Action->SetPatternInterval(11.f);

							Pattern10Sequence->PushBackChildren({ moveToCenter, pattern10Action });
						}
					}
					BehaviorSelector->SetRandomWeights({ 1.0 });
				}

				// MoveAction
				Condition* moveActionCondition = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(moveActionCondition);
				moveActionCondition->_successCondition = [this]() -> bool {
					Boss* boss = GetKey<Boss*>("Boss");
					Player* player = GetKey<Player*>("Player");
					float detectionRangeSquared = boss->Detection_Range * boss->Detection_Range;
					float playerDistanceSquared = (player->GetLocation() - boss->GetLocation()).LengthSquared();
					return detectionRangeSquared < playerDistanceSquared;
				};

				{
					//MoveAction
					Player* player = GetKey<Player*>("Player");
					MoveToLocation* moveToPlayer = CreateNode<MoveToLocation>();
					// 보스를 플레이어 위치로부터 감지 범위와 회피 범위 사이에 둡니다.
					float acceptableRadius = (boss->Detection_Range + boss->Avoidance_Range) / 2.f;
					// 하고 랜덤으로 살짝 흔듬.
					acceptableRadius += Random::Get((boss->Detection_Range - boss->Avoidance_Range) / 4.f);
					moveToPlayer->SetDestination(player->GetLocation());
					moveToPlayer->SetAcceptableRadius(acceptableRadius);

					moveActionCondition->Wrap(moveToPlayer);
				}

				//	Teleporting
				Condition* teleportingCondition = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(teleportingCondition);
				teleportingCondition->_successCondition = [this]() -> bool {
					Boss* boss = GetKey<Boss*>("Boss");
					Player* player = GetKey<Player*>("Player");
					float detectionRangeSquared = boss->Avoidance_Range * boss->Avoidance_Range;
					float playerDistanceSquared = (player->GetLocation() - boss->GetLocation()).LengthSquared();
					return playerDistanceSquared < detectionRangeSquared;
				};

				{
					// Teleporting
					DeclareKey<Math::Vector2>("TeleportDestination");
					Primer* setTeleportDestination = CreateNode<Primer>();
					setTeleportDestination->_action = [this]() {
						Math::Vector2 playerLocation = GetKey<Player*>("Player")->GetLocation();

						Math::Vector2 destinationCandidate1 = playerLocation + Math::Vector2{ 800, 0 };
						Math::Vector2 destinationCandidate2 = playerLocation - Math::Vector2{ 800, 0 };

						// TODO: 이동 가능한지 확인
						// 맵 마름모 꼴이니까 변마다 Line 만들고
						// Line{destination, BossCurrLoc} 가지고 
						// 교점 체크
						// -> 없으면 가능

						// 만약 둘다 가능하면 랜덤으로 결정
						// SetKey<Math::Vector2>("Pattern7Destination", destination);

						// 만약 둘다 안되면
						// ResetKey<Math::Vector2>("Pattern7Destination");
					};

					TeleportTo* teleportTo = CreateNode<TeleportTo>();
					teleportTo->SetObserveLocationKey("TeleportDestination");

					setTeleportDestination->Wrap(teleportTo);
					teleportingCondition->Wrap(setTeleportDestination);
				}

				//	BossPhase
				Selector* BossPhase = CreateNode<Selector>();
				BehaviorSelector->PushBackChild(BossPhase);
				{
					Condition* BossPhase_One = CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_One);
					BossPhase_One->_successCondition = [this]() -> bool
					{
						return (75 < GetKey<float>("Boss_HP") && GetKey<float>("Boss_HP") <= 100);
					};

					{
						// Boss Phase One
						RandomSelector* Phase_One_Selector = CreateNode<RandomSelector>();
						BossPhase_One->Wrap(Phase_One_Selector);
						{
							//Pattern02
							//Wait* Pattern02.DelayTime

						}
						{
							//	Phase_One_2
							Sequence* Phase_One_2 = CreateNode<Sequence>();
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
							Sequence* Phase_One_3 = CreateNode<Sequence>();
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
					Condition* BossPhase_Two = CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_Two);
					BossPhase_Two->_successCondition = [this]()->bool
						{
							return (25 < GetKey<float>("Boss_HP") && GetKey<float>("Boss_HP") <= 75);
						};
					{
						Selector* Phase_Pattern_Select = CreateNode<Selector>();
						BossPhase_Two->Wrap(Phase_Pattern_Select);
						DeclareKey<float>("Phase_Pattern_Cool_Time");
						SetKey<float>("Phase_Pattern_Cool_Time", boss->Phase_Pattern_Cool_Time);
						{
							// 2 Phase Periodic Pattern
							Condition* Phase_Two_Periodic = CreateNode<Condition>();
							Phase_Pattern_Select->PushBackChild(Phase_Two_Periodic);
							Phase_Two_Periodic->_successCondition = [this]()->bool
								{
									return (GetKey<float>("Phase_Pattern_Cool_Time") < 0.f);
								};
							{
								//Pattern09
								// Phase_Pattern_Cool_Time = Pattern19->DelayTime
							}
						}
						{
							// Boss Phase Two
							RandomSelector* Phase_Two_Selector = CreateNode<RandomSelector>();
							Phase_Pattern_Select->PushBackChild(Phase_Two_Selector);
							{
								//	Phase_Two_1
								Sequence* Phase_Two_1 = CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_1);
								{
									//Pattern02
									//Pattern02
									//Wait* Pattern02.DelayTime
								}
							}
							{
								//	Phase_Two_2
								Sequence* Phase_Two_2 = CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_2);
								{
									//Pattern03
									//Pattern04
									//Wait* Pattern04.DelayTime
								}
							}
							{
								//	Phase_Two_3
								Sequence* Phase_Two_3 = CreateNode<Sequence>();
								Phase_Two_Selector->PushBackChild(Phase_Two_3);
								{
									//Pattern05
									//Pattern08
									//Wait* Pattern08.DelayTime

								}
							}
							{
								//	Phase_Two_4
								Sequence* Phase_Two_4 = CreateNode<Sequence>();
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
					Condition* BossPhase_Three = CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_Three);
					BossPhase_Three->_successCondition = [this]()->bool
						{
							return (GetKey<float>("Boss_HP") <= 25);
						};
					{
						Selector* Phase_Pattern_Select = CreateNode<Selector>();
						BossPhase_Three->Wrap(Phase_Pattern_Select);
						{
							// 3 Phase Periodic Pattern 1
							Condition* Phase_Three_Periodic = CreateNode<Condition>();
							Phase_Pattern_Select->PushBackChild(Phase_Three_Periodic);
							Phase_Three_Periodic->_successCondition = [this]()->bool
								{
									return (GetKey<float>("Phase_Pattern_Cool_Time") < 0.f);
								};
							{
								// Phase_Three_Periodic_Selector
								RandomSelector* Phase_Three_Periodic_Selector = CreateNode<RandomSelector>();
								Phase_Pattern_Select->PushBackChild(Phase_Three_Periodic_Selector);
								{
									// 3 Phase Periodic Pattern 1
									Sequence* Phase_Three_1 = CreateNode<Sequence>();
									Phase_Three_Periodic_Selector->PushBackChild(Phase_Three_1);
									{
										//Pattern12
										{
											// Phase_Three_Periodic_1_RandomSelecor
											RandomSelector* Phase_Three_Periodic_1_RandomSelecor = CreateNode<RandomSelector>();
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
									Sequence* Phase_Three_2 = CreateNode<Sequence>();
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
							RandomSelector* Phase_Three_Selector = CreateNode<RandomSelector>();
							Phase_Pattern_Select->PushBackChild(Phase_Three_Selector);
							{
								//	Phase_Three_1
								Sequence* Phase_Three_1 = CreateNode<Sequence>();
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
								Sequence* Phase_Three_2 = CreateNode<Sequence>();
								Phase_Three_Selector->PushBackChild(Phase_Three_2);
								{
									//Pattern02
									//Pattern04
									//Wait* Pattern04.DelayTime
								}
							}
							{
								//	Phase_Three_3
								Sequence* Phase_Three_3 = CreateNode<Sequence>();
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
