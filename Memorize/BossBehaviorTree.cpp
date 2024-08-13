#include "BossBehaviorTree.h"

#include "D2DGameEngine/AIController.h"
#include "Boss.h"

BossBehaviorTree::BossBehaviorTree(Actor* _aiOwner) : BehaviorTree(_aiOwner)
{
	boss = static_cast<Boss*>(GetAIOwner()->GetPawn());
	//Keu Declarations�� ���� �𸣰���??
	Root* root = GetRoot();
	DeclareKey<Boss*>("Boss");
	SetKey<Boss*>("Boss", boss);
	{
		Selector* rootSelector = CreateNode<Selector>();

		{
			//	Groggy
			Condition* StateDie = CreateNode<Condition>();
			rootSelector->PushBackChild(StateDie);

			DeclareKey<int>("DissFellCount");
			SetKey<int>("DissFellCount", boss->DissfellCount);
			StateDie->_successCondition = [this]()->bool
				{
					return (GetKey<int>("DissFellCount") >= 10);
				};
			{
				//Groggy
			}
		}
		{
			// BehaviorSelector
			Selector* BehaviorSelector = CreateNode<Selector>();
			rootSelector->PushBackChild(BehaviorSelector);
			DeclareKey<float>("Detection_Range");
			SetKey<float>("Detection_Range", boss->Detection_Range);
			DeclareKey<float>("Avoidance_Range");
			SetKey<float>("Avoidance_Range", boss->Avoidance_Range);

			{
				//	Periodic_Pattern
				Condition* periodic_Pattern_Cool_Time = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(periodic_Pattern_Cool_Time);

				DeclareKey<float>("Periodic_Pattern_Cool_Time");
				SetKey<float>("Periodic_Pattern_Cool_Time", boss->Periodic_Pattern_Cool_Time);
				periodic_Pattern_Cool_Time->_successCondition = [this]()->bool
					{
						return (GetKey<int>("Periodic_Pattern_Cool_Time") < 0.f);
					};
				{
					RandomSelector* BehaviorSelector = CreateNode<RandomSelector>();
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
				Condition* moveActionCondition = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(moveActionCondition);
				moveActionCondition->_successCondition = [this]()->bool
					{
						return (GetKey<float>("Detection_Range") < GetKey<float>("Palyer_Distance"));
					};
				{
					//MoveACtion
				}

				//	Telproting
				Condition* telprotingCondition = CreateNode<Condition>();
				BehaviorSelector->PushBackChild(telprotingCondition);
				telprotingCondition->_successCondition = [this]()->bool
					{
						return (GetKey<float>("Palyer_Distance") < GetKey<float>("Avoidance_Range"));
					};
				{
					//Telproting
				}

				//	BossPhase
				Selector* BossPhase = CreateNode<Selector>();
				BehaviorSelector->PushBackChild(BossPhase);
				DeclareKey<float>("Boss_HP");
				SetKey<float>("Boss_HP", boss->hp);
				{
					Condition* BossPhase_One = CreateNode<Condition>();
					BossPhase->PushBackChild(BossPhase_One);
					BossPhase_One->_successCondition = [this]()->bool
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