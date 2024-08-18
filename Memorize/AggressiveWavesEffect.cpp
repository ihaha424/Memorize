#include "AggressiveWavesEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/BoxComponent.h"

AggressiveWavesEffect::AggressiveWavesEffect(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere02.png");
	endingState->SliceSpriteSheet(150, 150, 0, 0, 0, 0);
	endingState->FrameResize(7);
	endingState->SetFrameDurations({ 0.14285f });

	box->SetBoxExtent({ 130, 200 });

	bIsPassable = true;
	bEnding = false;
	bCollideWithOtherAttack = true;
}

AggressiveWavesEffect::~AggressiveWavesEffect()
{
}

void AggressiveWavesEffect::BeginPlay()
{
	__super::BeginPlay();
}

void AggressiveWavesEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	DamageEvent damageEvent;
	other->TakeDamage(damage, damageEvent, nullptr, this);
}

void AggressiveWavesEffect::Update(float _dt)
{
	__super::Update(_dt);
}
