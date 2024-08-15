#pragma once
#include "D2DGameEngine/SceneComponent.h"

class MagicCircle : public SceneComponent
{
	float distance = 150.f;

	class BitmapComponent* bm0;
public:
	MagicCircle(Actor* _owner);
	virtual ~MagicCircle();

	virtual void Update(float _dt) override;
};

