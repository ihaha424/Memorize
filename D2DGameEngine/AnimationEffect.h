#pragma once

#include "IEffect.h"

class AnimationBitmapComponent;

class AnimationEffect
	: public IEffect
{
public:
	AnimationEffect(class World* _world);
	virtual ~AnimationEffect() {}

	void SetSprite(std::wstring _name);
	AnimationBitmapComponent* GetAnimationBitmapComponent();

protected:
	AnimationBitmapComponent* animationBitMap;
	
};

