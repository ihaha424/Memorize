#include "BossBehaviorTree.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "BossAIController.h"
#include "Boss.h"
#include "Player.h"

#include "BossActionNodes.h"

BossBehaviorTree::BossBehaviorTree(Actor* _aiOwner) : BehaviorTree(_aiOwner)
{
	SetTickProperties(TICK_UPDATE);

	boss = static_cast<Boss*>(GetAIOwner()->GetPawn());
}

void BossBehaviorTree::BeginPlay()
{
	Super::BeginPlay();

	// ÇÃ·¹ÀÌ¾î Ã£¾Æ¼­ Æ÷ÀÎÅÍ·Î ³Ñ±è. Key·Î Á¢±Ù °¡´É
	DeclareKey<Player*>("Player");
	SetKey<Player*>("Player", GetWorld()->FindActorByType<Player>());

	BuildBehaviorTree();
	GetRoot()->Init();
}

void BossBehaviorTree::Update(float _dt)
{
	UpdateBehaviorTree(_dt);
}

void BossBehaviorTree::BuildBehaviorTree()
{
	Root* root = GetRoot();
	DeclareKey<Boss*>("Boss");
	boss = static_cast<BossAIController*>(GetOwner())->GetBoss();
	SetKey<Boss*>("Boss", boss);
	{
		Sequence* WaitAndGo = CreateNode<Sequence>();
		root->child = WaitAndGo;

		WaitForDelay* waitForDelay = CreateNode<WaitForDelay>();
		
		Selector* rootSelector = CreateNode<Selector>();

		WaitAndGo->PushBackChildren({ waitForDelay, rootSelector });

		{	// -> Root select 1
			//	Groggy
			rootSelector->PushBackChild(BuildPatternSubtree(Pattern::Groggy));
		}
		{	// -> Root select 2
			// mainBehaviorSelector
			Selector* mainBehaviorSelector = CreateNode<Selector>();
			rootSelector->PushBackChild(mainBehaviorSelector);

			{	// -> mainBehaviorSelector 1: Periodic Patterns
				// Main Behavior Branch
				Condition* IsCooledDown = CreateNode<Condition>();
				mainBehaviorSelector->PushBackChild(IsCooledDown);
				IsCooledDown->_successCondition = [this]() {
					// TODO: 시작한지 60 초 지났는지 체크하기.
					Boss* boss = GetKey<Boss*>("Boss");
					return boss->elapsedTime >= 60.f && boss->Periodic_Pattern_Cool_Time <= 0.f;
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
			{	// -> mainBehaviorSelector 2: Move
				// MoveAction
				INode* moveAction = BuildPatternSubtree(Pattern::Move);
				mainBehaviorSelector->PushBackChild(moveAction);
			}	// <- mainBehaviorSelector 2
			{	// -> mainBehaviorSelector 3: Teleport
				//	Teleporting Condition
				INode* teleportAction = BuildPatternSubtree(Pattern::Teleport);
				mainBehaviorSelector->PushBackChild(teleportAction);
			}	// <- mainBehaviorSelector 3
			{	// -> mainBehaviorSelector 4: Boss phases
				//	BossPhase
				RandomSelector* bossPhaseSelector = CreateNode<RandomSelector>();
				mainBehaviorSelector->PushBackChild(bossPhaseSelector);
				{	// -> bossPhaseSelector 1
					Condition* BossPhase_One = CreateNode<Condition>();
					bossPhaseSelector->PushBackChild(BossPhase_One);
					BossPhase_One->_successCondition = [this]() {
						float hpPercent = (float) boss->hp / boss->maxHp;
						return (hpPercent <= 1.f);
					};
					{	// Boss Phase One
						RandomSelector* phaseOneRandomSelector = CreateNode<RandomSelector>();
						BossPhase_One->Wrap(phaseOneRandomSelector);
						{	// -> phaseOneRandomSelector 1
							//Pattern02
							//Wait* Pattern02.DelayTime
							INode* pattern2 = BuildPatternSubtree(Pattern::Pattern2);
							phaseOneRandomSelector->PushBackChild(pattern2);
						}	// <- phaseOneRandomSelector 1
						{	// -> phaseOneRandomSelector 2
							//	Phase_One_2
							Sequence* Phase_One_2 = CreateNode<Sequence>();
							phaseOneRandomSelector->PushBackChild(Phase_One_2);
							{
								//Pattern05
								//Pattern03
								//Pattern04
								//Wait* Pattern04.DelayTime
								INode* pattern5 = BuildPatternSubtree(Pattern::Pattern5);
								INode* pattern3 = BuildPatternSubtree(Pattern::Pattern3);
								INode* pattern4 = BuildPatternSubtree(Pattern::Pattern4);
								Phase_One_2->PushBackChildren({ pattern5, pattern3, pattern4 });
							}
						}	// <- phaseOneRandomSelector 2
						{	// -> phaseOneRandomSelector 3
							//	Phase_One_3
							Sequence* Phase_One_3 = CreateNode<Sequence>();
							phaseOneRandomSelector->PushBackChild(Phase_One_3);
							{
								//Pattern05
								//Pattern08
								//Wait* Pattern08.DelayTime
								INode* pattern5 = BuildPatternSubtree(Pattern::Pattern5);
								INode* pattern8 = BuildPatternSubtree(Pattern::Pattern8);
								Phase_One_3->PushBackChildren({ pattern5, pattern8 });
							}
						}	// <- phaseOneRandomSelector 3
						{	// -> phaseOneRandomSelector 4
							//Pattern01
							//Wait* Pattern08.DelayTime
							INode* pattern1Action = BuildPatternSubtree(Pattern::Pattern1);
							phaseOneRandomSelector->PushBackChild(pattern1Action);
						}	// <- phaseOneRandomSelector 4
						phaseOneRandomSelector->SetRandomWeights({ 0.2, 0.2, 0.2, 0.35 });
					}	// END: Boss Phase One
				}	// <- bossPhaseSelector 1
				{	// -> bossPhaseSelector 2
					Condition* BossPhase_Two = CreateNode<Condition>();
					bossPhaseSelector->PushBackChild(BossPhase_Two);
					BossPhase_Two->_successCondition = [this]() {
						float hpPercent = (float)boss->hp / boss->maxHp;
						return (hpPercent <= 0.75f);
					};
					{
						Selector* Phase_Pattern_Select = CreateNode<Selector>();
						BossPhase_Two->Wrap(Phase_Pattern_Select);
						{
							// 2 Phase Periodic Pattern
							Condition* Phase_Two_Periodic = CreateNode<Condition>();
							Phase_Pattern_Select->PushBackChild(Phase_Two_Periodic);
							Phase_Two_Periodic->_successCondition = [this]() {
								return boss->Periodic_Pattern_Cool_Time <= 0.f;
							};
							//Pattern09
							// Phase_Pattern_Cool_Time = Pattern19->DelayTime
							INode* pattern09 = BuildPatternSubtree(Pattern::Pattern9);
							Phase_Two_Periodic->Wrap(pattern09);
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
									INode* pattern21 = BuildPatternSubtree(Pattern::Pattern2);
									INode* pattern22 = BuildPatternSubtree(Pattern::Pattern2);
									Phase_Two_1->PushBackChildren({ pattern21, pattern22 });
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
									INode* pattern3 = BuildPatternSubtree(Pattern::Pattern3);
									INode* pattern4 = BuildPatternSubtree(Pattern::Pattern4);
									Phase_Two_2->PushBackChildren({ pattern3, pattern4 });
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
									INode* pattern5 = BuildPatternSubtree(Pattern::Pattern5);
									INode* pattern8 = BuildPatternSubtree(Pattern::Pattern8);
									Phase_Two_3->PushBackChildren({ pattern5, pattern8 });
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
									INode* pattern5 = BuildPatternSubtree(Pattern::Pattern5);
									INode* pattern3 = BuildPatternSubtree(Pattern::Pattern3);
									Phase_Two_4->PushBackChildren({ pattern5, pattern3 });
								}
							}
							Phase_Two_Selector->SetRandomWeights({ 0.1f, 0.2f, 0.2f, 0.2f });
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
							Phase_Three_Periodic->_successCondition = [this]() {
								return boss->Phase_Pattern_Cool_Time < 0.f;
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
												INode* pattern8 = BuildPatternSubtree(Pattern::Pattern8);
												Phase_Three_Periodic_1_RandomSelecor->PushBackChild(pattern8);
											}
											{
												//Pattern09
												// Phase_Pattern_Cool_Time = Pattern12->DelayTime
												INode* pattern9 = BuildPatternSubtree(Pattern::Pattern9);
												Phase_Three_Periodic_1_RandomSelecor->PushBackChild(pattern9);
											}
											Phase_Three_Periodic_1_RandomSelecor->SetRandomWeights({ 1.f });
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
										INode* pattern13 = BuildPatternSubtree(Pattern::Pattern13);
										INode* pattern02_1 = BuildPatternSubtree(Pattern::Pattern2);
										INode* pattern02_2 = BuildPatternSubtree(Pattern::Pattern2);
										INode* pattern02_3 = BuildPatternSubtree(Pattern::Pattern2);
										SetPeriodicCoolTime* _20sec = CreateNode<SetPeriodicCoolTime>(20.f);
										Phase_Three_2->PushBackChildren({ pattern13, pattern02_1, pattern02_2, pattern02_3, _20sec });
									}
								}
								Phase_Three_Periodic_Selector->SetRandomWeights({ 1.f });
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
									INode* pattern9 = BuildPatternSubtree(Pattern::Pattern9);
									INode* pattern02_1 = BuildPatternSubtree(Pattern::Pattern2);
									INode* pattern02_2 = BuildPatternSubtree(Pattern::Pattern2);
									SetPeriodicCoolTime* _15sec = CreateNode<SetPeriodicCoolTime>(15.f);
									Phase_Three_1->PushBackChildren({ pattern9, pattern02_1, pattern02_2, _15sec });
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
									INode* pattern2 = BuildPatternSubtree(Pattern::Pattern2);
									INode* pattern4 = BuildPatternSubtree(Pattern::Pattern4);
									Phase_Three_2->PushBackChildren({ pattern2, pattern4 });
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
									INode* pattern11 = BuildPatternSubtree(Pattern::Pattern11);
									INode* pattern4_1 = BuildPatternSubtree(Pattern::Pattern4);
									INode* pattern4_2 = BuildPatternSubtree(Pattern::Pattern4);
									INode* pattern4_3 = BuildPatternSubtree(Pattern::Pattern4);
									Phase_Three_3->PushBackChildren({ pattern11, pattern4_1,pattern4_2, pattern4_3 });
								}
							}
							Phase_Three_Selector->SetRandomWeights({ 1.f });
						}
					}
				}	// <- bossPhaseSelector 3

				bossPhaseSelector->SetRandomWeights({ 3.f, 2.f, 1.f });
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
		ForceCondition* groggyCondition = CreateNode<ForceCondition>();
		// force if?
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

		DeclareKey<Math::Vector2>("MoveLocation");
		Primer* setMoveLocation = CreateNode<Primer>();
		moveActionCondition->Wrap(setMoveLocation);
		setMoveLocation->_action = [this]() {
			Boss* boss = GetKey<Boss*>("Boss");
			Player* player = GetKey<Player*>("Player");

			Math::Vector2 bossLocation = boss->GetLocation();
			Math::Vector2 playerLocation = player->GetLocation();

			// 보스를 플레이어 위치로부터 감지 범위와 회피 범위 사이에 둡니다.
			float acceptableRadius = (boss->Detection_Range + boss->Avoidance_Range) / 2.f;
			// 하고 랜덤으로 살짝 흔듬.
			acceptableRadius += Random::Get((boss->Detection_Range - boss->Avoidance_Range) / 4.f);
			
			Math::Vector2 fromPlayerToBoss = bossLocation - playerLocation;
			fromPlayerToBoss.Normalize();

			Math::Vector2 moveLocation = playerLocation + fromPlayerToBoss * acceptableRadius;

			while (!IsInMap(moveLocation))
			{
				Math::Matrix rotate45Degree = Math::Matrix::CreateRotationZ(PI_F / 4.f);

				moveLocation = Math::Vector2::Transform(moveLocation, rotate45Degree);
			}

			SetKey<Math::Vector2>("MoveLocation", moveLocation);
		};

		MoveTo* move = CreateNode<MoveTo>();
		setMoveLocation->Wrap(move);
		move->SetObserveLocationKey("MoveLocation");
		move->SetAcceptableRadius(50.f);
		move->SetSpeed(450.f);

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

			// 이동 가능한지 확인
			// 맵 마름모 꼴이니까 변마다 Line 만들고
			// Line{destination, BossCurrLoc} 가지고 
			// 교점 체크
			// -> 없으면 가능

			bool isDestinationCandidate1Available = IsInMap(destinationCandidate1);
			bool isDestinationCandidate2Available = IsInMap(destinationCandidate2);

			// 만약 둘다 가능하면 랜덤으로 결정
			if (isDestinationCandidate1Available && isDestinationCandidate2Available)
			{
				int r = Random::Get(1);
				if (r)
				{
					SetKey<Math::Vector2>("TeleportDestination", destinationCandidate1);
				}
				else
				{
					SetKey<Math::Vector2>("TeleportDestination", destinationCandidate2);
				}
			}
			else if (isDestinationCandidate1Available)
			{
				SetKey<Math::Vector2>("TeleportDestination", destinationCandidate1);
			}
			else if (isDestinationCandidate2Available)
			{
				SetKey<Math::Vector2>("TeleportDestination", destinationCandidate2);
			}
			else
			{
				// 만약 둘다 안되면
				ResetKey<Math::Vector2>("TeleportDestination");
			}
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
		Pattern1Action* pattern1Action = CreateNode<Pattern1Action>();
		pattern1Action->SetPatternInterval(1.f);
		pattern1Action->SetTreeCooldown(1.f);
		return pattern1Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern2: {
		Pattern2Action* pattern2Action = CreateNode<Pattern2Action>();
		pattern2Action->SetPatternInterval(2.0f);
		pattern2Action->SetTreeCooldown(2.f);
		return pattern2Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern3: {
		Pattern3Action* pattern3Action = CreateNode<Pattern3Action>();
		pattern3Action->SetPatternInterval(0.5f);
		pattern3Action->SetTreeCooldown(2.f);
		return pattern3Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern4: {
		Pattern4Action* pattern4Action = CreateNode<Pattern4Action>();
		pattern4Action->SetPatternInterval(0.5f);
		pattern4Action->SetTreeCooldown(2.f);
		return pattern4Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern5: {
		Sequence* pattern5Sequence = CreateNode<Sequence>();

		// Teleporting Logic
		DeclareKey<Math::Vector2>("pattern5TeleportDestination");
		Primer* setTeleportDestination = CreateNode<Primer>();
		setTeleportDestination->_action = [this]() {
			Math::Vector2 playerLocation = GetKey<Player*>("Player")->GetLocation();

			Math::Vector2 destinationCandidate1 = playerLocation + Math::Vector2{ 800, 0 };
			Math::Vector2 destinationCandidate2 = playerLocation - Math::Vector2{ 800, 0 };

			// 이동 가능한지 확인
			// 맵 마름모 꼴이니까 변마다 Line 만들고
			// Line{destination, BossCurrLoc} 가지고 
			// 교점 체크
			// -> 없으면 가능

			bool isDestinationCandidate1Available = IsInMap(destinationCandidate1);
			bool isDestinationCandidate2Available = IsInMap(destinationCandidate2);

			// 만약 둘다 가능하면 랜덤으로 결정
			if (isDestinationCandidate1Available && isDestinationCandidate2Available)
			{
				int r = Random::Get(1);
				if (r)
				{
					SetKey<Math::Vector2>("pattern5TeleportDestination", destinationCandidate1);
				}
				else
				{
					SetKey<Math::Vector2>("pattern5TeleportDestination", destinationCandidate2);
				}
			}
			else if (isDestinationCandidate1Available)
			{
				SetKey<Math::Vector2>("pattern5TeleportDestination", destinationCandidate1);
			}
			else if (isDestinationCandidate2Available)
			{
				SetKey<Math::Vector2>("pattern5TeleportDestination", destinationCandidate2);
			}
			else
			{
				// 만약 둘다 안되면
				ResetKey<Math::Vector2>("pattern5TeleportDestination");
			}
		};

		TeleportTo* teleportTo = CreateNode<TeleportTo>();
		teleportTo->SetObserveLocationKey("pattern5TeleportDestination");

		DeclareKey<Math::Vector2>("Pattern5Destination");
		Primer* setDestination = CreateNode<Primer>();
		setDestination->_action = [this]() {
			if (!IsKeySet<Math::Vector2>("pattern5TeleportDestination"))
			{
				ResetKey<Math::Vector2>("Pattern5Destination");
				return;
			}

			Math::Vector2 teleportLocation = GetKey<Math::Vector2>("pattern5TeleportDestination");

			Math::Vector2 destinationCandidate1 = teleportLocation + Math::Vector2{ 0, 710 };
			Math::Vector2 destinationCandidate2 = teleportLocation - Math::Vector2{ 0, 710 };

			// 이동 가능한지 확인
			// 맵 마름모 꼴이니까 변마다 Line 만들고
			// Line{destination, BossCurrLoc} 가지고 
			// 교점 체크
			// -> 없으면 가능
			bool isDestinationCandidate1Available = IsInMap(destinationCandidate1);
			bool isDestinationCandidate2Available = IsInMap(destinationCandidate2);

			// 만약 둘다 가능하면 랜덤으로 결정
			if (isDestinationCandidate1Available && isDestinationCandidate2Available)
			{
				int r = Random::Get(1);
				if (r)
				{
					SetKey<Math::Vector2>("Pattern5Destination", destinationCandidate1);
				}
				else
				{
					SetKey<Math::Vector2>("Pattern5Destination", destinationCandidate2);
				}
			}
			else if (isDestinationCandidate1Available)
			{
				SetKey<Math::Vector2>("Pattern5Destination", destinationCandidate1);
			}
			else if (isDestinationCandidate2Available)
			{
				SetKey<Math::Vector2>("Pattern5Destination", destinationCandidate2);
			}
			else
			{
				// 만약 둘다 안되면
				ResetKey<Math::Vector2>("Pattern5Destination");
			}
		};

		Pattern5Action* pattern5Action = CreateNode<Pattern5Action>();
		pattern5Action->SetPatternInterval(3.f);
		pattern5Action->speed = 250.f;
		pattern5Action->castingInterval = 1.f;

		pattern5Sequence->PushBackChildren({ setTeleportDestination, setDestination });
		setTeleportDestination->Wrap(teleportTo);
		setDestination->Wrap(pattern5Action);

		return pattern5Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern6: {
		//Pattern06
		Sequence* Pattern6Sequence = CreateNode<Sequence>();

		MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
		moveToCenter->SetAcceptableRadius(10.f);
		moveToCenter->SetSpeed(900.f);
		moveToCenter->SetDestination(GET_MAP_CENTER());

		Pattern6Action* pattern6Action = CreateNode<Pattern6Action>();
		pattern6Action->SetCooldown(11.f + 15.f);
		pattern6Action->SetPatternInterval(11.f);
		pattern6Action->SetTreeCooldown(3.f);

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

			// 이동 가능한지 확인
			// 맵 마름모 꼴이니까 변마다 Line 만들고
			// Line{destination, BossCurrLoc} 가지고 
			// 교점 체크
			// -> 없으면 가능
			bool isDestinationCandidate1Available = IsInMap(destinationCandidate1);
			bool isDestinationCandidate2Available = IsInMap(destinationCandidate2);

			// 만약 둘다 가능하면 랜덤으로 결정
			if (isDestinationCandidate1Available && isDestinationCandidate2Available)
			{
				int r = Random::Get(1);
				if (r)
				{
					SetKey<Math::Vector2>("Pattern7Destination", destinationCandidate1);
				}
				else
				{
					SetKey<Math::Vector2>("Pattern7Destination", destinationCandidate2);
				}
			}
			else if (isDestinationCandidate1Available)
			{
				SetKey<Math::Vector2>("Pattern7Destination", destinationCandidate1);
			}
			else if (isDestinationCandidate2Available)
			{
				SetKey<Math::Vector2>("Pattern7Destination", destinationCandidate2);
			}
			else
			{
				// 만약 둘다 안되면
				// ResetKey<Math::Vector2>("Pattern7Destination");
				ResetKey<Math::Vector2>("Pattern7Destination");
			}
		};

		// Selector to find the correct position.
		MoveTo* moveToPlayer = CreateNode<MoveTo>();
		moveToPlayer->SetObserveLocationKey("Pattern7Destination");
		moveToPlayer->SetAcceptableRadius(10.f);
		moveToPlayer->SetSpeed(900.f);

		setDestination->Wrap(moveToPlayer);

		Pattern7Action* pattern7Action = CreateNode<Pattern7Action>();
		pattern7Action->SetCooldown(20.f + 5.f);
		pattern7Action->SetPatternInterval(5.f);
		pattern7Action->SetTreeCooldown(4.f);

		Pattern7Sequence->PushBackChildren({ setDestination, pattern7Action });
		return Pattern7Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern8: {
		Pattern8Action* pattern8Action = CreateNode<Pattern8Action>();
		pattern8Action->SetPatternInterval(0.5f);
		pattern8Action->SetTreeCooldown(1.f);
		return pattern8Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern9: {
		Pattern9Action* pattern9Action = CreateNode<Pattern9Action>();
		pattern9Action->SetCooldown(15.f + 5.f);
		pattern9Action->SetPatternInterval(5.f);
		pattern9Action->SetTreeCooldown(2.f);
		return pattern9Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern10: {
		//Pattern10
		Sequence* Pattern10Sequence = CreateNode<Sequence>();

		MoveToLocation* moveToCenter = CreateNode<MoveToLocation>();
		moveToCenter->SetAcceptableRadius(10.f);
		moveToCenter->SetSpeed(900.f);
		moveToCenter->SetDestination(GET_MAP_CENTER());	// 센터로 이동

		Pattern10Action* pattern10Action = CreateNode<Pattern10Action>();
		pattern10Action->SetCooldown(20.f + 11.f);
		pattern10Action->SetPatternInterval(11.f);
		pattern10Action->SetTreeCooldown(5.f);

		Pattern10Sequence->PushBackChildren({ moveToCenter, pattern10Action });
		return Pattern10Sequence;
	} break;
	case BossBehaviorTree::Pattern::Pattern11: {
		Pattern11Action* pattern11Action = CreateNode<Pattern11Action>();
		pattern11Action->SetPatternInterval(2.f);
		return pattern11Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern12: {
		Pattern12Action* pattern12Action = CreateNode<Pattern12Action>();
		pattern12Action->SetCooldown(25.f);
		pattern12Action->SetPatternInterval(1.f);
		return pattern12Action;
	} break;
	case BossBehaviorTree::Pattern::Pattern13: {
		Pattern13Action* pattern13Action = CreateNode<Pattern13Action>();
		pattern13Action->SetCooldown(20.f);
		pattern13Action->SetPatternInterval(1.f);
		pattern13Action->SetTreeCooldown(3.f);
		return pattern13Action;
	} break;
	default:
		break;
	}

	OBJ_ERROR(-1, "Unknown pattern!");
	return nullptr;
}
