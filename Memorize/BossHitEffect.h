#pragma once

#include "../D2DGameEngine/AnimatorEffect.h"

class BossHitEffect
	:public AnimatorEffect
{
public:
	BossHitEffect(class World* _world);
	virtual ~BossHitEffect() {}

	void SetEffect(int index);

};

