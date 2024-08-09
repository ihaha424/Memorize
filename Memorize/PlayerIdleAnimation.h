#pragma once

#include "D2DGameEngine/AnimationState.h"

class PlayerIdleAnimation :
	public AnimationState
{
	using Super = AnimationState;
public:
	PlayerIdleAnimation(SceneComponent* _owner) : Super(_owner) {}
	~PlayerIdleAnimation() {};

	virtual void Toggle(Animator*) override;
};

