#pragma once

#include "INode.h"
#include "RandomGenerator.h"

struct Composite : public INode {
	std::vector<INode*> children;
	void PushBackChild(INode* node) {
		children.push_back(node);
	}
	void PushBackChildren(std::initializer_list<INode*> llist) {
		for (INode* e : llist) {
			children.push_back(e);
		}
	}
};

struct Sequence : public Composite {
	std::size_t i;

	virtual void Init() override {
		status = NodeStatus::Ready;
		i = 0;
	}

	virtual void Traverse(float dt) override {
		for (; i < children.size(); ++i) {
			INode* child = children[i];
			child->Traverse(dt);
			switch (child->status) {
			case NodeStatus::Failure: {
				status = NodeStatus::Failure;
				return;
			}
			case NodeStatus::Running: {
				status = NodeStatus::Running;
				return;
			}
			case NodeStatus::Success:
				break;
			}
		}
		// If all succeeded
		status = NodeStatus::Success;
	}
};

struct Selector : public Composite {
	std::size_t i;

	virtual void Init() override {
		status = NodeStatus::Ready;
		i = 0;
	}

	virtual void Traverse(float dt) override {
		for (; i < children.size(); ++i) {
			INode* child = children[i];
			child->Traverse(dt);
			switch (child->status) {
			case NodeStatus::Failure:
				break;
			case NodeStatus::Running: {
				status = NodeStatus::Running;
				return;
			}
			case NodeStatus::Success: {
				status = NodeStatus::Success;
				return;
			}
			}
		}
		status = NodeStatus::Failure;
	}
};

struct RandomSelector : public Composite {
	std::size_t i;

	std::vector<INode*> randomOrder;
	std::vector<double> randomWeights;
	virtual void SetRandomWeights(std::initializer_list<double> weights)
	{
		randomWeights.assign(weights);
	}

	virtual void Init() override {
		status = NodeStatus::Ready;
		std::shuffle(children.begin(), children.end(), Random::Engine());
	}

	virtual void Traverse(float dt) override {
		for (; i < children.size(); ++i) {
			INode* child = children[i];
			child->Traverse(dt);
			switch (child->status) {
			case NodeStatus::Failure: {
				break;
			}
			case NodeStatus::Running: {
				status = NodeStatus::Running;
				return;
			}
			case NodeStatus::Success: {
				status = NodeStatus::Success;
				return;
			}
			}
		}
		status = NodeStatus::Failure;
	}
};

struct Parallel : public Composite {
	std::size_t i;

	virtual void Init() override {
		status = NodeStatus::Ready;
		i = 0;
	}

	virtual void Traverse(float dt) override {
		bool anyRunning = false;
		bool allSuccess = true;
		for (; i < children.size(); ++i) {
			INode* child = children[i];
			child->Traverse(dt);
			switch (child->status) {
			case NodeStatus::Running:
				anyRunning = true;
				allSuccess = false;
				break;
			case NodeStatus::Failure:
				allSuccess = false;
				break;
			case NodeStatus::Success:
				break;
			}
		}
		if (allSuccess) {
			status = NodeStatus::Success;
		}
		else if (anyRunning) {
			status = NodeStatus::Running;
		}
		else {
			status = NodeStatus::Failure;
		}
	}
};