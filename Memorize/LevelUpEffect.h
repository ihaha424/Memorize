#pragma once

#include "../D2DGameEngine/AnimatorEffect.h"
#include "D2DGameEngine/DotTween.h"

class LevelUpEffect
	:public AnimatorEffect
{
public:
	LevelUpEffect(class World* _world);
	virtual ~LevelUpEffect();

	void SetEffect(int index);

	virtual void Update(float _dt) override;

	DotTween<float>* verticalTween;
	float vertical;
	DotTween<float>* scaleTween;
	float scale;
	Math::Vector2 pos;

	virtual void BeginPlay() override;
};

