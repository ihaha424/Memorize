#include "AnimationState.h"

#include "Animator.h"

#include "AnimationBitmapComponent.h"

AnimationState::AnimationState(AnimationBitmapComponent* animation)
	: _animation{ animation } {}

void AnimationState::Exit(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// _animation = _animationSceneRegistry["idle_exit"];
	_animation->Trigger(false);
}

void AnimationState::Toggle(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// if (controller->GetVariable("Jump") == true) {
	//   controller->SetState(PlayerJump);
	// } else if (controller->GetVariable("Duck") == true) {
	//   controller->SetState(PlayerDuck);
	// }
	//
}

void AnimationState::Enter(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// _animation = _animationSceneRegistry["idle_exit"];
	_animation->Trigger(true);
}

void AnimationState::AddAnimationStateDependency(
	const StateName& name,
	AnimationState* state) {

	auto it = _stateDependency.find(name);
	if (it != _stateDependency.end()) {
		throw std::invalid_argument(
			"The state \'" + name +
			"\' already exists!");
	}

	_stateDependency.insert(
		std::make_pair(name, state)
	);
}
