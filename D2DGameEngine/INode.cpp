#include "INode.h"

#include "Pawn.h"
#include "BehaviorTree.h"
#include "AIController.h"

class Pawn* INode::GetPawn() { return GetAIController()->GetPawn(); }

BehaviorTree* INode::GetBehaviorTree()
{
	return bt;
}

AIController* INode::GetAIController() { return bt->aiOwner; }
