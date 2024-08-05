#pragma once

#include "framework.h"

#include "INode.h"
#include "Composite.h"
#include "Decorator.h"
#include "Leaf.h"
#include "Wait.h"
#include "MoveTo.h"

class GameObject;
class AIController;
class BehaviorTree {
	friend struct INode;
	friend class AIController;
	using KeyName = std::string;
	using KeyValue = std::any;
	using Context = std::unordered_map<KeyName, KeyValue>;
	using NodeRegistry = std::vector<INode*>;

	GameObject* _owner;
	AIController* _controller;

	NodeRegistry _nodeRegistry;
	Root* _root;

	NodeStatus _lastStatus;

	Context _keys;

public:
	BehaviorTree() :
		_owner{ nullptr },
		_controller{ nullptr },
		_lastStatus{ NodeStatus::Ready } {
		_root = CreateNode<Root>();
	}
	~BehaviorTree() {
		for (INode* e : _nodeRegistry) delete e;
	}

	Root* GetRoot() { return _root; }

	void UpdateBehaviorTree(float dt);

	template<NodeType T, typename... Args>
	T* CreateNode(Args&&... args) {
		T* node = new T(std::forward<Args>(args)...);
		node->myBT = this;
		_nodeRegistry.push_back(node);
		return node;
	}

	// Key Modifiers
	template<typename T>
	bool DeclareKey(KeyName var) {
		auto it = _keys.find(var);
		if (it != _keys.end()) {
			return false;
		}
		_keys.insert({ var, std::make_any<std::optional<T>>() });
		return true;
	}

	template<typename T>
	bool IsKeySet(KeyName var) {
		auto it = _keys.find(var);
		if (it == _keys.end()) {
			return false;
		}

		try {	// Type check
			std::optional<T>& op = std::any_cast<std::optional<T>&>(it->second);
			return op.has_value();
		}
		catch (const std::bad_any_cast& e) {
			// TODO: need to log.
			return false;
		}
		return false;
	}

	template<typename T>
	void ResetKey(KeyName var) {
		auto it = _keys.find(var);
		if (it == _keys.end()) {
			throw std::invalid_argument("BehaviorTree: Key doesn't exist!");
		}

		std::optional<T>& op = std::any_cast<std::optional<T>&>(it->second);
		op.reset();
	}

	template<typename T>
	bool SetKey(KeyName var, const T& val) {
		// Check if the variable exists first.
		auto it = _keys.find(var);
		if (it == _keys.end()) {	// Variable not found
			// TODO: need to log.
			return false;
		}

		// Change the value of the variable
		try {	// Type check
			std::optional<T>& op = std::any_cast<std::optional<T>&>(it->second);
			op = val;
			return true;
		}
		catch (const std::bad_any_cast& e) {
			// TODO: need to log.
			return false;
		}
	}

	template<typename T>
	T& GetKey(KeyName var) {
		// Check if the variable exists.
		auto it = _keys.find(var);
		if (it == _keys.end()) {
			throw std::invalid_argument("BehaviorTree: Key doesn't exist!");
		}

		// Type check
		std::optional<T>& op = std::any_cast<std::optional<T>&>(it->second);	// < reference
		return op.value();
	}

private:
	NodeStatus TraverseFromRoot(float dt);
};
