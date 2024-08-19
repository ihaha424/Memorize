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
		{
			const std::type_info& self = typeid(*this);
			std::string className = self.name();
			size_t classPos = className.find("class ");
			if (classPos != std::string::npos) {
				className.erase(classPos, 6); // 6 is the length of "class "
			}
			//std::cout << "run: " << className<<"\n";
		}
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
