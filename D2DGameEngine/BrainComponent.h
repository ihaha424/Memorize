#pragma once

#include "IComponent.h"
#include "IEventListener.h"

#include "Debug.h"

class AIController;
class BrainComponent : public IComponent, public IEventListener
{
	LOG_REGISTER_OBJ(BrainComponent)
	using KeyName = std::string;
	using KeyValue = std::any;
	using Context = std::unordered_map<KeyName, KeyValue>;
protected:
	AIController* aiOwner;

	Context _keys;
public:
	BrainComponent(Actor* _aiOwner);

	/**
	 * @brief Stop all activity and clean up the logic
	 */

	AIController* GetAIOwner() { return aiOwner; }
	class Pawn* GetPawn();

	virtual void Cleanup() {}

	virtual bool IsPaused() { return false; }
	virtual bool IsRunning() { return false; }

	virtual void StartLogic() {}
	virtual void PauseLogic() {}
	virtual void ResumeLogic() {}
	virtual void RestartLogic() {}
	virtual void StopLogic() {}

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
	bool DeleteKey(KeyName var) {
		auto it = _keys.find(var);
		if (it != _keys.end()) {
			return true;
		}
		_keys.erase(it);
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

};
