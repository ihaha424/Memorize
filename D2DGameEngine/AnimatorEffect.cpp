#include "AnimatorEffect.h"
#include "Animator.h"

AnimatorEffect::AnimatorEffect(World* _world)
	: IEffect(_world)
{
	animatorBitMap = CreateComponent<Animator>();
	rootComponent = animatorBitMap;
}
