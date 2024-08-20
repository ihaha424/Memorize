#pragma once
#include "../D2DGameEngine/AnimatorEffect.h"


class MemorizeEffect : public AnimatorEffect
{
public:

	MemorizeEffect(class World* _world);
	virtual ~MemorizeEffect() {}

	void StartEffect();
	void SetPosition(Math::Vector2* _pos);

	virtual void FixedUpdate(float _fixedRate) override;

private:
	Math::Vector2* playerPos;
};

