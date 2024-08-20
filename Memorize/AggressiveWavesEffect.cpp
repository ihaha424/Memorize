#include "AggressiveWavesEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/PolygonComponent.h"

AggressiveWavesEffect::AggressiveWavesEffect(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_AggressiveWaves.png");
	normalState->SliceSpriteSheet(1024, 1024, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	box->SetPolygon({ {-200, 400}, {200, -400}, {-200, -400}, {200, 400} });

	bIsPassable = true;
	bEnding = false;
	bHasEnding = false;
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
