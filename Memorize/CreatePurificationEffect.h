#pragma once

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/AnimationEffect.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

inline 
AnimationEffect* CreatePurificationEffect(World* _world, Math::Vector2 position, float _scale) {

	AnimationEffect* purificationEffect = _world->GetEffectSystem().CreateEffect<AnimationEffect>();
	AnimationBitmapComponent* purificationAnimation = purificationEffect->GetAnimationBitmapComponent();

	purificationAnimation->SetSprite(L"TestResource/Player/Skill/Skill_Purification.png");
	purificationAnimation->SetScale(_scale, _scale);
	purificationAnimation->SliceSpriteSheet(1000, 1000, 0, 0, 0, 0);
	purificationAnimation->SetFrameDurations({ 2.0f / 60 });
	purificationAnimation->Trigger(true);
	purificationEffect-> SetAliveTime(2.0f);
	purificationEffect->SetLocation(position.x, position.y);

	return purificationEffect;
}

