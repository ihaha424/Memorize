#pragma once

#include "D2DGameEngine/AnimationState.h"

class PlayerMoveAnimation :
    public AnimationState
{
	using Super = AnimationState;
public:
	PlayerMoveAnimation(SceneComponent* _owner) : Super(_owner) {}
	~PlayerMoveAnimation() {};

	virtual void Toggle(Animator*) override;
};

