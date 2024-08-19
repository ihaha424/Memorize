#pragma once

#include "IEffect.h"

class AnimationEffect
	: public IEffect
{
public:
	AnimationEffect(class World* _world);
	virtual ~AnimationEffect() {}


protected:
	class AnimationBitmapComponent* animationBitMap;
	
};

