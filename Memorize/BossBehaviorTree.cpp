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
	Root* root = GetRoot();
	DeclareKey<Boss*>("Boss");
	SetKey<Boss*>("Boss", boss);
	{
		Selector* rootSelector = CreateNode<Selector>();

		{	// -> Root select 1
			//	Groggy
			rootSelector->PushBackChild(BuildPatternSubtree(Pattern::Groggy));
		}
		{	// -> Root select 2
			// mainBehaviorSelector
			Selector* mainBehaviorSelector = CreateNode<Selector>();
			rootSelector->PushBackChild(mainBehaviorSelector);

			{	// -> mainBehaviorSelector 1
				// Main Behavior Branch
				Condition* IsCooledDown = CreateNode<Condition>();
				mainBehaviorSelector->PushBackChild(IsCooledDown);
				IsCooledDown->_successCondition = [this]() {
					// TODO: 시작한지 60 초 지났는지 체크하기.
					Boss* boss = GetKey<Boss*>("Boss");
					return boss->Periodic_Pattern_Cool_Time <= 0.f;
				};
				{	// Periodic Pattern Selector
					RandomSelector* periodicPatternSelector = CreateNode<RandomSelector>();
					IsCooledDown->Wrap(periodicPatternSelector);
					{	// -> periodicPatternSelector 1
						//Pattern06
						INode* pattern06 = BuildPatternSubtree(Pattern::Pattern6);
						periodicPatternSelector->PushBackChild(pattern06);
					}
					{	// -> periodicPatternSelector 2
						//Pattern07
						INode* pattern07 = BuildPatternSubtree(Pattern::Pattern7);
						periodicPatternSelector->PushBackChild(pattern07);
					}
					{	// -> BehaviorSelector 3
						//Pattern10
						INode* pattern10 = BuildPatternSubtree(Pattern::Pattern10);
						periodicPatternSelector->PushBackChild(pattern10);
					}
					// Set equal weights
					periodicPatternSelector->SetRandomWeights({ 1.0 });
				}
			}	// <- mainBehaviorSelector 1 
			{	// -> mainBehaviorSelector 2
				// MoveAction
				INode* moveAction = BuildPatternSubtree(Pattern::Move);
				mainBehaviorSelector->PushBackChild(moveAction);
			}	// <- mainBehaviorSelector 2
			{	// -> mainBehaviorSelector 3
				//	Teleporting Condition
				INode* teleportAction = BuildPatternSubtree(Pattern::Teleport);
				mainBehaviorSelector->PushBackChild(teleportAction);
			}	// <- mainBehaviorSelector 3
			{	// -> mainBehaviorSelector 4
				//	BossPhase
				Selector* bossPhaseSelector = CreateNode<Selector>();
				mainBehaviorSelector->PushBackChild(bossPhaseSelector);
				{	// -> bossPhaseSelector 1
					Condition* BossPhase_One = CreateNode<Condition>();
					bossPhaseSelector->PushBackChild(BossPhase_One);
					BossPhase_One->_successCondition = [this]() {
						float hpPercent = (float) boss->hp / boss->maxHp;
						return (0.75f < hpPercent && hpPercent <= 1.f);
					};
					{	// Boss Phase One
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
				}	// <- bossPhaseSelector 1
				{	// -> bossPhaseSelector 2
					Condition* BossPhase_Two = CreateNode<Condition>();
					bossPhaseSelector->PushBackChild(BossPhase_Two);
					BossPhase_Two->_successCondition = [this]() {
						float hpPercent = (float)boss->hp / boss->maxHp;
						return (0.25f < hpPercent && hpPercent <= 0.75f);
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
				}	// <- bossPhaseSelector 2
				{	// -> bossPhaseSelector 3
					Condition* BossPhase_Three = CreateNode<Condition>();
					bossPhaseSelector->PushBackChild(BossPhase_Three);
					BossPhase_Three->_successCondition = [this]() {
						float hpPercent = (float)boss->hp / boss->maxHp;
						return hpPercent <= 0.25f;
					};
					{	// Phase3 Pattern Selector
						Selector* phase3PatternSelector = CreateNode<Selector>();
						BossPhase_Three->Wrap(phase3PatternSelector);
						{
							// 3 Phase Periodic Pattern 1
							Condition* Phase_Three_Periodic = CreateNode<Condition>();
							phase3PatternSelector->PushBackChild(Phase_Three_Periodic);
							Phase_Three_Periodic->_successCondition = [this]()->bool
								{
									return (GetKey<float>("Phase_Pattern_Cool_Time") < 0.f);
								};
							{	
								// Phase_Three_Periodic_Selector
								RandomSelector* Phase_Three_Periodic_Selector = CreateNode<RandomSelector>();
								phase3PatternSelector->PushBackChild(Phase_Three_Periodic_Selector);
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
							phase3PatternSelector->PushBackChild(Phase_Three_Selector);
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
				}	// <- bossPhaseSelector 3
			}
		}
	}
}

INode* BossBehaviorTree::BuildPatternSubtree(Pattern pattern)
{
	switch (pattern)
	{
	case BossBehaviorTree::Pattern::Groggy: {
		//	Groggy
		Condition* groggyCondition = CreateNode<Condition>();
		// TODO: force if?
		groggyCondition->_successCondition = [this]() {
			Boss* boss = GetKey<Boss*>("Boss");
			return (boss->DissfellCount >= 10);
		};
		{	// Groggy
			Sequence* groggySequence = CreateNode<Sequence>();
			groggyCondition->Wrap(groggySequence);
			{
				// GroggyAction
				GroggyAction* groggyAction = CreateNode<GroggyAction>();
				groggySequence->PushBackChild(groggyAction);

				// Wait for 10
				Wait* groggyWaitFor10Seconds = CreateNode<Wait>(5);
				groggySequence->PushBackChild(groggyWaitFor10Seconds);
			}
		}
		return groggyCondition;
	} break;
	case Pattern::Move: {
		// MoveAction
		Condition* moveActionCondition = CreateNode<Condition>();
		moveActionCondition->_successCondition = [this]() -> bool {
			Boss* boss = GetKey<Boss*>("Boss");
			Player* player = GetKey<Player*>("Player");
			float detectionRangeSquared = boss->Detection_Range * boss->Detection_Range;
			float playerDistanceSquared = (player->GetLocation() - boss->GetLocation()).LengthSquared();
			return detectionRangeSquared < playerDistanceSquared;
		};
		Player* player = GetKey<Player*>("Player");
		MoveToLocation* moveToPlayer = CreateNode<MoveToLocation>();
		// 보스를 플레이어 위치로부터 감지 범위와 회피 범위 사이에 둡니다.
		float acceptableRadius = (boss->Detection_Range + boss->Avoidance_Range) / 2.f;
		// 하고 랜덤으로 살짝 흔듬.
		acceptableRadius += Random::Get((boss->Detection_Range - boss->Avoidance_Range) / 4.f);
		moveToPlayer->SetDestination(player->GetLocation());
		moveToPlayer->SetAcceptableRadius(acceptableRadius);

		moveActionCondition->Wrap(moveToPlayer);
		return moveActionCondition;
	}	break;
	case Pattern::Teleport: {
		//	Teleporting Condition
		Condition* teleportCondition = CreateNode<Condition>();
		teleportCondition->_successCondition = [this]() -> bool {
			Boss* boss = GetKey<Boss*>("Boss");
			Player* player = GetKey<Player*>("Player");
			float detectionRangeSquared = boss->Avoidance_Range * boss->Avoidance_Range;
			float playerDistanceSquared = (player->GetLocation() - boss->GetLocation()).LengthSquared();
			return playerDistanceSquared < detectionRangeSquared;
		};

		// Teleporting Logic
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
		teleportCondition->Wrap(setTeleportDestination);

		/*		 
					텔레포트 로직
		
				teleportCondition
(플레이어가 AvoidanceRange 안으로 들어옴)
								|
								V
			setTeleportDestination
	(텔레포트 위치가 벽이 아닌지 확인)
								|
								V
						teleport
	(텔레포트 위치가 이동 가능하면 텔레포트)
		
		*/
		return teleportCondition;
	} break;
	case BossBehaviorTree::Pattern::Pattern1: {
	} break;
	case BossBehaviorTree::Pattern::Pattern2: {
		Pattern2Action* pattern2Action = CreateNode<Pattern2Action>();
		pattern2Action->SetPatternInterval(1.0f);
		return pattern2Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern3: {
		Pattern3Action* pattern3Action = CreateNode<Pattern3Action>();
		pattern3Action->SetPatternInterval(0.5f);
		return pattern3Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern4: {
		Pattern4Action* pattern4Action = CreateNode<Pattern4Action>();
		pattern4Action->SetPatternInterval(0.5f);
		return pattern4Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern5: {
	} break;
	case BossBehaviorTree::Pattern::Pattern6: {
		//Pattern06
		Sequence* Pattern6Sequence = CreateNode<Sequence>();

		MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
		moveToCenter->SetAcceptableRadius(10.f);
		moveToCenter->SetSpeed(900.f);
		moveToCenter->SetDestination(/*TODO:센터 값 조절 요*/{ 2014, 1050 });

		Pattern6Action* pattern6Action = CreateNode<Pattern6Action>();
		pattern6Action->SetCooldown(15.f);
		pattern6Action->SetPatternInterval(11.f);

		Pattern6Sequence->PushBackChildren({ moveToCenter, pattern6Action });
		return Pattern6Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern7: {
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
		return Pattern7Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern8: {
	} break;
	case BossBehaviorTree::Pattern::Pattern9: {
	} break;
	case BossBehaviorTree::Pattern::Pattern10: {
		//Pattern10
		Sequence* Pattern10Sequence = CreateNode<Sequence>();

		MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
		moveToCenter->SetAcceptableRadius(10.f);
		moveToCenter->SetSpeed(900.f);
		moveToCenter->SetDestination(/*TODO:센터 값 조절 요*/{ 2014, 1050 });

		Pattern10Action* pattern10Action = CreateNode<Pattern10Action>();
		pattern10Action->SetCooldown(20.f);
		pattern10Action->SetPatternInterval(11.f);

		Pattern10Sequence->PushBackChildren({ moveToCenter, pattern10Action });
		return Pattern10Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern11: {
	} break;
	case BossBehaviorTree::Pattern::Pattern12: {
	} break;
	case BossBehaviorTree::Pattern::Pattern13: {
	} break;
	default:
		break;
	}

	OBJ_ERROR(-1, "Unknown pattern!");
	return nullptr;
}
