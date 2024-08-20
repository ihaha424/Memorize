#pragma once
#include "../D2DGameEngine/AnimatorEffect.h"


class HeadEffect : public AnimatorEffect
{
public:

	HeadEffect(class World* _world);
	virtual ~HeadEffect() {}

	void SetEffect(int index);
	void SetPosition(Math::Vector2* _pos);

	virtual void FixedUpdate(float _fixedRate) override;

private:
	Math::Vector2* playerPos;
};

