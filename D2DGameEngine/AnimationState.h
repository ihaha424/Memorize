#pragma once

#include "framework.h"

class AnimationBitmapComponent;
class Animator;
class AnimationState {
	friend class Animator;
	using StateName = std::string;
	using StateDependency = std::unordered_map<StateName, AnimationState*>;

protected:
	StateDependency _stateDependency;
	AnimationBitmapComponent* _animation;

public:
	AnimationState(AnimationBitmapComponent* animation);
	virtual ~AnimationState() {}
	virtual void Exit(Animator*);
	virtual void Toggle(Animator*);
	virtual void Enter(Animator*);
	void AddAnimationStateDependency(
		const StateName&,
		AnimationState*
	);
};
