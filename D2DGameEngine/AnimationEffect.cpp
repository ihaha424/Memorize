#include "AnimationEffect.h"
#include "AnimationBitmapComponent.h"

AnimationEffect::AnimationEffect(World* _world)
	: IEffect(_world)
{
	animationBitMap = CreateComponent<AnimationBitmapComponent>();
	rootComponent = animationBitMap;

}
