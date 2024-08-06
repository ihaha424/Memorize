#include "BehaviorTree.h"

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
