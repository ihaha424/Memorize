#pragma once
#include "RangeSkill.h"
class TighteningCircle : public RangeSkill
{
	float tighteningTime = 2.f;
	float elapsedTime = 0.f;
	Actor* circle = nullptr;
	BitmapComponent* circleBitmap = nullptr;
	CapsuleComponent* circleShape = nullptr;
public:
	TighteningCircle(Actor* _owner);
	virtual ~TighteningCircle();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	virtual void UseSkill() override;

};

