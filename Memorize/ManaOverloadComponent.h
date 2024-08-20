#pragma once
#include "D2DGameEngine/AnimationBitmapComponent.h"

class ManaOverloadComponent : public AnimationBitmapComponent
{
	float elapsedTime = 0.f;
	float duration = 10.f;
	class GPlayerController* controller;
public:
	ManaOverloadComponent(Actor* _owner);
	virtual ~ManaOverloadComponent();

	virtual void Update(float _dt) override;
};

