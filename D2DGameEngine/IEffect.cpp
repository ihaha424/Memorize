#include "IEffect.h"

void IEffect::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	aliveTime -= _fixedRate;
	if (aliveTime <= 0.f)
	{
		Destroy();
	}
}
