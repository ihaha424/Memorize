#pragma once

#include "INode.h"

struct Leaf : public INode {};

struct Action : public Leaf {
	virtual void Init() override {
		status = NodeStatus::Ready;
		started = false;
		failed = false;
	}
	virtual void Run(float dt) = 0;
	virtual bool IsRunning() = 0;
	virtual bool IsFailed() { return failed; };
	virtual void Traverse(float dt) override {
		Run(dt);

		if (IsRunning()) {
			status = NodeStatus::Running;
		}
		else if (IsFailed()) {
			status = NodeStatus::Failure;
		}
		else {
			status = NodeStatus::Success;
		}
	}

protected:
	bool started;
	bool failed;
};
