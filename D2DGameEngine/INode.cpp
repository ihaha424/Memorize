#include "INode.h"

#include "BehaviorTree.h"

GameObject* INode::GetOwner() { return myBT->_owner; }

AIController* INode::GetController() { return myBT->_controller; }
