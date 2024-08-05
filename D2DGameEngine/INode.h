#pragma once

#include "framework.h"

enum class NodeStatus {
	Ready,
	Failure,
	Running,
	Success
};

class GameObject;
class AIController;
class BehaviorTree;
struct INode {
	BehaviorTree* myBT;
	NodeStatus status;
	virtual void Init() { status = NodeStatus::Ready; }
	virtual void Traverse(float dt) = 0;
	GameObject* GetOwner();
	AIController* GetController();
};

template<class T>
concept NodeType = std::is_base_of<INode, T>::value;

struct Root final : public INode {
	INode* child;

	void Init() override {
		status = NodeStatus::Ready;
	}

	void Traverse(float dt) override {
		child->Traverse(dt);
		status = child->status;
	}
};
