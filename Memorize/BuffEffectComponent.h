#pragma once
#include "D2DGameEngine/Animator.h"
class BuffEffectComponent : public Animator
{
	float duration = 0.5f;
	float elapsedTime = 0.f;
	class Player* player;

public:
	BuffEffectComponent(Actor* _owner);
	virtual ~BuffEffectComponent();

	virtual void Update(float _dt) override;

	void SetDuration(float _duration) { duration = _duration; };
};

