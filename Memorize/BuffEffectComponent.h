#pragma once
#include "D2DGameEngine/AnimationBitmapComponent.h"
class BuffEffectComponent : public AnimationBitmapComponent
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

