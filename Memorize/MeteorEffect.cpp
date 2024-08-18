#include "MeteorEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "MovementComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"

MeteorEffect::MeteorEffect(World* _world) : Projectile(_world)
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

	//box collider 재설정
	box->SetCollisionObjectType(ECollisionChannel::PlayerPattern);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Overlap);
	box->SetStatus(OS_INACTIVE);

	duration = 10.f;
	bIsPassable = true;
	endingTime = 1.f;
}

MeteorEffect::~MeteorEffect()
{
}

void MeteorEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void MeteorEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	DamageEvent damageEvent;
	other->TakeDamage(damage, damageEvent, nullptr, this);
}

void MeteorEffect::Update(float _dt)
{
	__super::Update(_dt);

	box->bShouldOverlapTest = true;

	if (Math::Vector2::Distance(GetLocation(), attackPos) < 20)
	{
		if (bEnding) return;
		//위치에 도달하면 폭발하고 collider를 켠다.
		mv->SetSpeed(0);
		anim->SetState(endingState);
		box->SetStatus(OS_ACTIVE);
		bEnding = true;
		elapsedTime = duration + delay;
	}
}

void MeteorEffect::Initialize()
{
	__super::Initialize();

	anim->SetState(normalState);
}
