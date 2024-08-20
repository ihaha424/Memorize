#include "AnimationEffect.h"
#include "AnimationBitmapComponent.h"

AnimationEffect::AnimationEffect(World* _world)
	: IEffect(_world)
{
	animationBitMap = CreateComponent<AnimationBitmapComponent>();
	rootComponent = animationBitMap;

}

void AnimationEffect::SetSprite(std::wstring _path)
{
	animationBitMap->SetSprite(_path.c_str());
}

AnimationBitmapComponent* AnimationEffect::GetAnimationBitmapComponent()
{
	return animationBitMap;
}