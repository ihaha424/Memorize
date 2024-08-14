#pragma once

#include "common.h"

#include "D2DGameEngine/BehaviorTree.h"

class BossBehaviorTree : public BehaviorTree
{
	using Super = BehaviorTree;
	LOG_REGISTER_OBJ(BossBehaviorTree)

	class Boss* boss{ nullptr };
public:
	BossBehaviorTree(Actor* _aiOwner);

	void BeginPlay() override;
	void Update(float _dt) override;
private:
	void BuildBehaviorTree();

	enum class Pattern {
		Groggy,
		Move,
		Teleport,
		Pattern1,
		Pattern2,
		Pattern3,
		Pattern4,
		Pattern5,
		Pattern6,
		Pattern7,
		Pattern8,
		Pattern9,
		Pattern10,
		Pattern11,
		Pattern12,
		Pattern13,
	};

	INode* BuildPatternSubtree(Pattern pattern);
};
