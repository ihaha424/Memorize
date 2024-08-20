#pragma once

#include "IEffect.h"

class AnimatorEffect
	: public IEffect
{
public:
	AnimatorEffect(class World* _world);
	virtual ~AnimatorEffect() {}


protected:
	class Animator* animatorBitMap;

};

