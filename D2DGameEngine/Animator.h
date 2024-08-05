#pragma once

#include "framework.h"

#include "IComponent.h"

class AnimationBitmapComponent;
class AnimationState;
class Animator : public IComponent {
	using VariableName = std::string;
	using VariableValue = std::any;
	using AnimationVariables = std::unordered_map<VariableName, std::any>;
	using StateRegistry = std::vector<AnimationState*>;

	AnimationVariables _variables;
	StateRegistry _animationStates;
	AnimationState* _currState;
public:
	Animator(AnimationState* state) : _currState(state) {
		SetTickProperties(TICK_UPDATE | TICK_RENDER);
	}

	/**
	 * @brief 현재 애니메이션을 토글합니다.
	 */
	void Toggle();
	/**
	 * @brief 현재 애니메이션 상태를 바꿉니다.
	 * @param state 
	 */
	void SetState(AnimationState* state);

	// Animation Controller Variable Modifiers
	template<typename T>
	bool DeclareVariable(VariableName var, const T& init = T{}) {
		auto it = _variables.find(var);
		if (it != _variables.end()) {
			return false;
		}
		_variables.insert({ var, std::any(init) });
		return true;
	}

	template<typename T>
	bool SetVariable(VariableName var, const T& val, bool toggle = false) {
		// Check if the variable exists first.
		auto it = _variables.find(var);
		if (it == _variables.end()) {	// Variable not found
			// TODO: need to log.
			return false;
		}

		// Change the value of the variable
		try {	// Type check
			T& v = std::any_cast<T&>(it->second);
			v = val;

			// If toggle is true, the current state reacts to 
			// the variable changes.
			if (toggle) Toggle();
			return true;
		}
		catch (const std::bad_any_cast& e) {
			// TODO: need to log.
			return false;
		}
	}

	template<typename T>
	T& GetVariable(VariableName var) {
		// Check if the variable exists.
		auto it = _variables.find(var);
		if (it == _variables.end()) {
			throw std::invalid_argument("Animator: Variable doesn't exist!");
		}

		// Type check
		T& v = std::any_cast<T&>(it->second);	// < reference
		return v;
	}

	// Getters
	AnimationBitmapComponent* GetCurrentAnimationScene();

	void Update(float _dt) override;
	void Render(class D2DRenderer* _renderer) override;
};
