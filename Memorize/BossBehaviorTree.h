#pragma once

#include "common.h"

#include "D2DGameEngine/BehaviorTree.h"

class BossBehaviorTree : public BehaviorTree
{
	class Boss* boss{ nullptr };
public:
	BossBehaviorTree(Actor* _aiOwner);


};
