#pragma once

#include "INode.h"

struct Decorator : public INode {
	INode* child;
	void Wrap(INode* node) { child = node; }
};

struct Inverter : public Decorator {
	virtual void Traverse(float dt) override {
		child->Traverse(dt);

		switch (child->status) {
		case NodeStatus::Failure: {
			status = NodeStatus::Success;
			break;
		}
		case NodeStatus::Running: {
			status = NodeStatus::Running;
			break;
		}
		case NodeStatus::Success: {
			status = NodeStatus::Failure;
			break;
		}
		}
	}
};

struct Succeeder : public Decorator {
	virtual void Traverse(float dt) override {
		child->Traverse(dt);
		status = NodeStatus::Success;
	}
};

struct Repeater : public Decorator {
	std::size_t numRepeats;
	std::size_t next;

	Repeater() : Repeater(-1) {}
	Repeater(std::size_t numRepeats)
		: numRepeats{ numRepeats }, next{ 0 } {}

	virtual void Init() override {
		status = NodeStatus::Ready;
		next = 0;
	}

	virtual void Traverse(float dt) override {
		NodeStatus res = NodeStatus::Success;
		for (; next < numRepeats; ++next) {
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
			case NodeStatus::Success: {
				++next;
				break;
			}
			}
		}
		status = NodeStatus::Success;
	}
};

struct Condition : public Decorator {
	std::function<bool()> _successCondition;
	virtual void Traverse(float dt) override {
		if (status == NodeStatus::Running)
		{
			child->Traverse(dt);
			status = child->status;
		}
		else 
		{
			if (!_successCondition()) 
			{
				status = NodeStatus::Failure;
				return;
			}

			child->Traverse(dt);
			status = child->status;
		}
	}
};

struct ForceCondition : public Decorator {
	std::function<bool()> _successCondition;
	virtual void Traverse(float dt) override {
		if (!_successCondition())
		{
			status = NodeStatus::Failure;
			return;
		}

		child->Traverse(dt);
		status = child->status;
	}
};

struct Primer : public Decorator {
	std::function<void()> _action;
	virtual void Traverse(float dt) override {
		if (status == NodeStatus::Running)
		{
			child->Traverse(dt);
			status = child->status;
		}
		else
		{
			_action();
			child->Traverse(dt);
			status = child->status;
		}
	}
};
