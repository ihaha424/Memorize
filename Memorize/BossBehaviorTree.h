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
};
