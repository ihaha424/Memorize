#include "BehaviorTree.h"

BehaviorTree::BehaviorTree(Actor* _aiOwner) :
	BrainComponent{ _aiOwner },
	_lastStatus{ NodeStatus::Ready } {
	_root = CreateNode<Root>();
}

void BehaviorTree::UpdateBehaviorTree(float dt) {
	_lastStatus = TraverseFromRoot(dt);
}

NodeStatus BehaviorTree::TraverseFromRoot(float dt) {
	_root->Traverse(dt);
	if (_root->status != NodeStatus::Running) {
		for (INode* node : _nodeRegistry) {
			node->Init();
		}
	}
	return _root->status;
}
