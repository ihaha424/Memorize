#pragma once

#include "BrainComponent.h"

#include "INode.h"
#include "Composite.h"
#include "Decorator.h"
#include "Leaf.h"
#include "Wait.h"
#include "MoveTo.h"

class GameObject;
class AIController;
class BehaviorTree : public BrainComponent {
	friend struct INode;
	friend class AIController;
	
	using NodeRegistry = std::vector<INode*>;

	NodeRegistry _nodeRegistry;
	Root* _root;

	bool bIsRunning{ false };
	bool bIsPaused{ false };
	NodeStatus _lastStatus;

public:
	BehaviorTree(Actor* _aiOwner);
	~BehaviorTree() {
		for (INode* e : _nodeRegistry) delete e;
	}
	
	Root* GetRoot() { return _root; }

	void UpdateBehaviorTree(float dt);

	template<NodeType T, typename... Args>
	T* CreateNode(Args&&... args) {
		T* node = new T(std::forward<Args>(args)...);
		node->bt = this;
		_nodeRegistry.push_back(node);
		return node;
	}

	// Overridden from BrainComponent
	virtual void Cleanup() {
	
	}

	virtual bool IsPaused() { return false; }
	virtual bool IsRunning() { return false; }

	virtual void StartLogic() {}
	virtual void PauseLogic() {}
	virtual void ResumeLogic() {}
	virtual void RestartLogic() {}
	virtual void StopLogic() {}

private:
	NodeStatus TraverseFromRoot(float dt);
};
